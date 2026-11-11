#!/usr/bin/env python3
#
# Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
#

"""Parse a Valgrind Memcheck XML report and print a Markdown summary.

By default, the report includes every error recorded in the XML file. Repeated
``--source-dir`` options restrict the reported errors to those with at least one
stack frame whose source file is under one of the specified directories. This
filters the report and its exit status; it does not limit what Valgrind checks.
Heap and Valgrind summary totals remain process-wide. The report is always
written to standard output. When ``--source-dir`` is used, that output and the
exit status are filtered; ``--output`` saves the full, unfiltered report.

Examples:
    python3 scripts/check_valgrind.py memcheck.xml
    python3 scripts/check_valgrind.py memcheck.xml \\
        --source-dir /workspace/mito --source-dir /workspace/pyre
    python3 scripts/check_valgrind.py memcheck.xml \\
        --source-dir /workspace/mito --output memcheck-full-report.md

The script exits with status 1 when the selected report contains errors, or 0
when it does not. Use ``--help`` for the complete command-line interface.
"""

import argparse
from collections import Counter
from pathlib import Path
import sys
import xml.etree.ElementTree as ET


CURRENT_XML_PROTOCOL = "6"


def text(element, path, default=""):
    if element is None:
        return default
    
    value = element.findtext(path, default=default)
    return value.strip() if value else default


def integer(element, path):
    try:
        return int(text(element, path, "0").replace(",", ""))
    except ValueError:
        return 0

    
def numeric_values(element):
    if element is None:
        return {}
    return {
        child.tag: integer(element, child.tag)
        for child in element
        if child.text and child.text.strip()
    }


def categorized_values(element):
    if element is None:
        return {}
    return {
        child.tag: {
            "bytes": integer(child, "bytes"),
            "blocks": integer(child, "blocks"),
        }
        for child in element
    }


def location(frame):
    function = text(frame, "fn")
    filename = text(frame, "file")
    directory = text(frame, "dir")
    line = text(frame, "line")
    obj = text(frame, "obj")

    if filename:
        source = f"{directory}/{filename}" if directory else filename

        if line:
            source += f":{line}"
        return f"{function} ({source})" if function else source
    
    if function:
        return f"{function} ({obj})" if obj else function
    
    return obj or "unknown location"


def source_path(frame):
    filename = text(frame, "file")
    if not filename:
        return None

    directory = text(frame, "dir")
    path = Path(directory) / filename
    if not path.is_absolute():
        return None
    return path.resolve()


def has_source_frame(error, source_dirs):
    for frame in error.findall("./stack/frame"):
        path = source_path(frame)
        if path is not None and any(
            path.is_relative_to(source_dir) for source_dir in source_dirs
        ):
            return True
    return False


def parse_report(xml_path, source_dirs=()):
    root = ET.parse(xml_path).getroot()
    xml_errors = root.findall("error")
    errors = (
        [error for error in xml_errors if has_source_frame(error, source_dirs)]
        if source_dirs
        else xml_errors
    )
    statuses = root.findall("status")
    final_status = statuses[-1] if statuses else None
    occurrences = {
        text(pair, "unique"): integer(pair, "count")
        for pair in root.findall("./errorcounts/pair")
    }

    details = []
    for index, error in enumerate(errors, start=1):
        unique = text(error, "unique")
        kind = text(error, "kind", "Unknown")
        message = text(error, "xwhat/text") or text(error, "what", "No description")
        frames = [location(frame) for frame in error.findall("./stack/frame")]
        auxiliary = [
            text(auxwhat, "text")
            for auxwhat in error.findall("auxwhat")
            if text(auxwhat, "text")
        ]
        suppression = error.find("suppression")
        details.append(
            {
                "number": index,
                "unique": unique,
                "thread_id": text(error, "tid"),
                "kind": kind,
                "message": message,
                "count": occurrences.get(unique, 1),
                "frames": frames,
                "auxiliary": auxiliary,
                "leaked_bytes": integer(error, "xwhat/leakedbytes"),
                "leaked_blocks": integer(error, "xwhat/leakedblocks"),
                "suppression": {
                    "name": text(suppression, "sname"),
                    "kind": text(suppression, "skind"),
                    "details": text(suppression, "skaux"),
                }
                if suppression is not None
                else {},
            }
        )

    return {
        "tool": text(root, "protocoltool", "unknown"),
        "protocol": text(root, "protocolversion", "unknown"),
        "state": text(final_status, "state", "unknown"),
        "duration": text(final_status, "time"),
        "errors": details,
        "total_errors": len(xml_errors),
        "source_dirs": [str(source_dir) for source_dir in source_dirs],
        "heap": numeric_values(root.find("heap_summary/memory_in_use_at_exit")),
        "heap_usage": {
            key: integer(root, f"heap_summary/total_heap_usage/{key}")
            for key in ("allocs", "frees", "bytes_allocated")
        },
        "leak_summary": categorized_values(root.find("leak_summary")),
        "error_summary": (
            {
                key: integer(root, f"error_summary/{key}")
                for key in ("errors", "error_contexts", "suppressed", "suppressed_contexts")
            }
            if root.find("error_summary") is not None
            else None
        ),
        "suppression_counts": [
            {"name": text(pair, "name"), "count": integer(pair, "count")}
            for pair in root.findall("./suppcounts/pair")
        ],
        "all_heap_blocks_freed": text(root, "all_heap_blocks_freed", "unknown"),
    }


def format_bytes(value):
    if value < 1024:
        return f"{value} B"
    if value < 1024**2:
        return f"{value / 1024:.1f} KiB"
    if value < 1024**3:
        return f"{value / 1024**2:.1f} MiB"
    return f"{value / 1024**3:.1f} GiB"


def render_report(report, source):
    errors = report["errors"]
    kinds = Counter(error["kind"] for error in errors)
    total_occurrences = sum(error["count"] for error in errors)
    lines = [
        "# Memcheck report",
        "",
        f"- **Input:** `{source}`",
        f"- **Tool:** {report['tool']}",
        f"- **XML protocol:** {report['protocol']}",
        f"- **Run status:** {report['state']}",
        (
            f"- **Distinct errors in selected sources:** {len(errors)}"
            if report["source_dirs"]
            else f"- **Distinct errors:** {len(errors)}"
        ),
        f"- **Total occurrences:** {total_occurrences}",
        "",
    ]

    if report["source_dirs"]:
        lines[-1:-1] = [
            "- **Source directories:** "
            + ", ".join(f"`{path}`" for path in report["source_dirs"]),
            f"- **Distinct errors before source filtering:** {report['total_errors']}",
            "- Error details and exit status include an error only when its stack "
            "has a source file under one of the selected directories.",
            "- Valgrind still checks the entire process; heap and Valgrind summary "
            "totals below are process-wide.",
        ]

    if report["duration"]:
        lines.insert(-1, f"- **Elapsed time:** {report['duration'].strip()}")
    
    if report["heap"] or report["heap_usage"]:
        lines.extend(["## Heap summary", ""])
        
        if report["heap"]:
            lines.append(
                f"- **At exit:** {report['heap'].get('blocks', 0):,} blocks, "
                f"{format_bytes(report['heap'].get('bytes', 0))}"
            )
        usage = report["heap_usage"]
        
        if any(usage.values()):
            lines.append(
                f"- **Total allocations/frees:** {usage['allocs']:,} / {usage['frees']:,}"
            )
            lines.append(
                f"- **Total bytes allocated:** {format_bytes(usage['bytes_allocated'])}"
            )
        lines.append(
            f"- **All heap blocks freed:** {report['all_heap_blocks_freed']}"
        )
        lines.append("")
    
    leak_summary = report["leak_summary"]
    if leak_summary:
        lines.extend(
            [
                "## Valgrind leak summary"
                + (" (entire process)" if report["source_dirs"] else ""),
                "",
                "| Category | Blocks | Bytes |",
                "|---|---:|---:|",
            ]
        )
        for category, values in leak_summary.items():
            lines.append(
                f"| `{category}` | {values['blocks']:,} | {format_bytes(values['bytes'])} |"
            )
        lines.append("")

    error_summary = report["error_summary"]
    if error_summary is not None:
        lines.extend(
            [
                "## Valgrind error summary"
                + (" (entire process)" if report["source_dirs"] else ""),
                "",
            ]
        )
        lines.extend(
            f"- **{label}:** {error_summary[key]:,}"
            for key, label in (
                ("errors", "Errors"),
                ("error_contexts", "Error contexts"),
                ("suppressed", "Suppressed errors"),
                ("suppressed_contexts", "Suppressed contexts"),
            )
        )
        lines.append("")

    if report["suppression_counts"]:
        lines.extend(["## Suppressions used", ""])
        lines.extend(
            f"- **{item['name']}:** {item['count']:,}"
            for item in report["suppression_counts"]
        )
        lines.append("")

    if kinds:
        lines.extend(["## Errors by kind", ""])
        lines.extend(
            f"- **{kind}:** {count} distinct ({sum(e['count'] for e in errors if e['kind'] == kind)} occurrences)"
            for kind, count in sorted(kinds.items())
        )
        lines.append("")

    leak_errors = [
        error for error in errors if error["kind"].startswith("Leak_")
    ]
    if leak_errors:
        lines.extend(["## Leak summary", "", "| Category | Blocks | Bytes |", "|---|---:|---:|"])
        for kind in sorted({error["kind"] for error in leak_errors}):
            matching = [error for error in leak_errors if error["kind"] == kind]
            blocks = sum(error["leaked_blocks"] for error in matching)
            byte_count = sum(error["leaked_bytes"] for error in matching)
            lines.append(f"| `{kind}` | {blocks} | {format_bytes(byte_count)} |")
        lines.append("")

    if not errors:
        lines.extend(
            [
                (
                    "No Memcheck errors were found in the selected source directories."
                    if report["source_dirs"]
                    else "No Memcheck errors were found."
                ),
                "",
            ]
        )
        return "\n".join(lines)
    
    lines.extend(["## Error details", ""])
    for error in errors:
        lines.extend(
            [
                f"### {error['number']}. {error['kind']} — {error['count']} occurrence(s)",
                "",
                error["message"],
                "",
            ]
        )

        if error["unique"]:
            lines.extend([f"- **Unique ID:** `{error['unique']}`"])

        if error["thread_id"]:
            lines.append(f"- **Thread ID:** {error['thread_id']}")

        if error["leaked_bytes"] or error["leaked_blocks"]:
            lines.append(
                f"- **Leak size:** {error['leaked_blocks']} block(s), "
                f"{format_bytes(error['leaked_bytes'])}"
            )

        for auxiliary in error["auxiliary"]:
            lines.append(f"- **Additional information:** {auxiliary}")

        suppression = error["suppression"]
        if suppression:
            description = ", ".join(
                value
                for value in (suppression["kind"], suppression["details"])
                if value
            )
            lines.append(
                f"- **Suppression:** `{suppression['name'] or 'unnamed'}`"
                + (f" ({description})" if description else "")
            )

        if error["frames"]:
            lines.extend(["", "**Stack trace:**"])
            lines.extend(f"- `{frame}`" for frame in error["frames"])
        lines.append("")

    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(
        description="Analyze a Valgrind Memcheck XML file and print a Markdown report."
    )
    parser.add_argument("xml_file", type=Path, help="path to memcheck.xml")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        help="also save the full, unfiltered report to this file",
    )
    parser.add_argument(
        "--source-dir",
        type=Path,
        action="append",
        default=[],
        help=(
            "include only errors with a stack frame sourced under this directory; "
            "may be specified multiple times"
        ),
    )

    args = parser.parse_args()
    source_dirs = [source_dir.resolve() for source_dir in args.source_dir]
    try:
        report = parse_report(args.xml_file, source_dirs)
    except (OSError, ET.ParseError) as error:
        parser.error(f"cannot read {args.xml_file}: {error}")

    if report["protocol"] != CURRENT_XML_PROTOCOL:
        print(
            "WARNING: Memcheck XML protocol "
            f"{report['protocol']} differs from the version this script expects "
            f"({CURRENT_XML_PROTOCOL}); review the report and update the parser "
            "if needed.",
            file=sys.stderr,
        )

    markdown = render_report(report, args.xml_file)
    if args.output:
        try:
            full_report = parse_report(args.xml_file) if source_dirs else report
        except (OSError, ET.ParseError) as error:
            parser.error(f"cannot read {args.xml_file}: {error}")

        try:
            args.output.write_text(
                render_report(full_report, args.xml_file),
                encoding="utf-8",
            )
        except OSError as error:
            parser.error(f"cannot write {args.output}: {error}")
    sys.stdout.write(markdown)

    return 1 if report["errors"] else 0


if __name__ == "__main__":
    sys.exit(main())