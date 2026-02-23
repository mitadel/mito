#!/bin/bash
#
# Build and run all C++ tutorials in the tutorial/ directory.
# Captures stdout and artifacts into docs/_tutorial_outputs/<tutorial>/ for inclusion in Sphinx docs.
#
# Usage: .github/scripts/build_and_run_tutorials.sh
#
# This script is called by the CI workflow (.github/workflows/docs.yaml) before building docs.
# It can also be run locally to test tutorial rendering.
#

set -e

REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
TUTORIAL_BASE_DIR="$REPO_ROOT/tutorial"
DOCS_OUTPUT_DIR="$REPO_ROOT/docs/_tutorial_outputs"
STATIC_OUTPUT_DIR="$REPO_ROOT/docs/_static/tutorials"
BUILD_BASE_DIR="$REPO_ROOT/build/docs_tutorials"

# Create output directories
mkdir -p "$DOCS_OUTPUT_DIR"
mkdir -p "$STATIC_OUTPUT_DIR"

# Counter for reporting
TUTORIAL_COUNT=0
BUILT_COUNT=0
FAILED_COUNT=0

echo "=========================================="
echo "Building and running C++ tutorials..."
echo "=========================================="
echo "Repo root: $REPO_ROOT"
echo "Tutorial dir: $TUTORIAL_BASE_DIR"
echo "Output dir: $DOCS_OUTPUT_DIR"
echo ""

# Iterate over each tutorial subdirectory
if [ ! -d "$TUTORIAL_BASE_DIR" ]; then
  echo "Warning: tutorial directory not found at $TUTORIAL_BASE_DIR"
  echo "Skipping tutorial builds."
  exit 0
fi

for tutorial_dir in "$TUTORIAL_BASE_DIR"/*; do
  if [ ! -d "$tutorial_dir" ]; then
    continue
  fi

  tutorial_name=$(basename "$tutorial_dir")

  # Check if CMakeLists.txt exists
  if [ ! -f "$tutorial_dir/CMakeLists.txt" ]; then
    echo "Skipping $tutorial_name (no CMakeLists.txt found)"
    continue
  fi

  TUTORIAL_COUNT=$((TUTORIAL_COUNT + 1))
  echo ""
  echo "--- Building tutorial: $tutorial_name ---"

  BUILD_DIR="$BUILD_BASE_DIR/$tutorial_name"
  OUTPUT_DIR="$DOCS_OUTPUT_DIR/$tutorial_name"

  # Clean and create build directory
  rm -rf "$BUILD_DIR"
  mkdir -p "$BUILD_DIR"
  mkdir -p "$OUTPUT_DIR"

  # Configure CMake
  echo "Configuring CMake..."
  if ! cmake -S "$tutorial_dir" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release 2>&1 | tail -20; then
    echo "Error: CMake configuration failed for $tutorial_name"
    FAILED_COUNT=$((FAILED_COUNT + 1))
    continue
  fi

  # Build
  echo "Building..."
  if ! cmake --build "$BUILD_DIR" -- -j$(nproc) 2>&1 | tail -20; then
    echo "Error: Build failed for $tutorial_name"
    FAILED_COUNT=$((FAILED_COUNT + 1))
    continue
  fi

  BUILT_COUNT=$((BUILT_COUNT + 1))

  # Find and run the executable
  if [ -x "$BUILD_DIR/main" ]; then
    exe="$BUILD_DIR/main"
  else
    # Try to find the first executable in the build directory
    exe=$(find "$BUILD_DIR" -maxdepth 1 -type f -executable | head -n 1 || echo "")
  fi

  if [ -n "$exe" ] && [ -x "$exe" ]; then
    echo "Running: $exe"
    # Run with timeout (300 seconds) and capture output
    if timeout 300 "$exe" > "$OUTPUT_DIR/output.txt" 2>&1; then
      echo "✓ Successfully ran $tutorial_name"
    else
      EXIT_CODE=$?
      if [ $EXIT_CODE -eq 124 ]; then
        echo "⚠ Tutorial $tutorial_name timed out (>300 seconds)"
        echo "Timeout error" > "$OUTPUT_DIR/output.txt"
      else
        echo "⚠ Tutorial $tutorial_name exited with code $EXIT_CODE"
      fi
    fi
  else
    echo "Warning: No executable found for $tutorial_name"
  fi

  # Copy generated artifacts (VTU, PNG, etc.) to output directory
  echo "Copying artifacts..."
  find "$BUILD_DIR" -maxdepth 2 -type f \( -name "*.vtu" -o -name "*.pvtu" -o -name "*.png" -o -name "*.pdf" \) -exec cp -t "$OUTPUT_DIR" -- {} + 2>/dev/null || true

  # Also copy to _static for direct image serving
  find "$BUILD_DIR" -maxdepth 2 -type f \( -name "*.png" -o -name "*.svg" \) -exec cp -t "$STATIC_OUTPUT_DIR" -- {} + 2>/dev/null || true

done

echo ""
echo "=========================================="
echo "Tutorial build summary:"
echo "  Total tutorials found: $TUTORIAL_COUNT"
echo "  Successfully built: $BUILT_COUNT"
echo "  Failed: $FAILED_COUNT"
echo "  Outputs saved to: $DOCS_OUTPUT_DIR"
echo "=========================================="

# Make outputs readable by web server
chmod -R a+r "$DOCS_OUTPUT_DIR" || true

# Exit with error if any tutorial failed
if [ $FAILED_COUNT -gt 0 ]; then
  echo ""
  echo "Error: $FAILED_COUNT tutorial(s) failed to build"
  exit 1
fi

exit 0
