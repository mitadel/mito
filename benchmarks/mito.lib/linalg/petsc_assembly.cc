// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// get the benchmark library
#include <benchmark/benchmark.h>
// get the mito library
#include <mito.h>


// types
using index_t = mito::linalg::petsc::index_t;
using scalar_t = mito::linalg::petsc::scalar_t;
using format_t = mito::linalg::assembly_format;
using segment_t = mito::linalg::block_segment_t;

// the assembly formats under test
constexpr std::array<std::pair<const char *, format_t>, 3> formats = { {
    { "coo", format_t::coo },
    { "csr", format_t::csr },
    { "hash", format_t::hash },
} };

// the flat assembly input of one rank
struct lists_t {
    std::vector<index_t> rows;
    std::vector<index_t> cols;
    std::vector<scalar_t> vals;
    std::vector<segment_t> segments;
};

// a reporter that reports nothing, used on all ranks but rank 0
class NullReporter : public benchmark::BenchmarkReporter {
  public:
    auto ReportContext(const Context &) -> bool override { return true; }
    auto ReportRuns(const std::vector<Run> &) -> void override {}
};


// the rank of this process and the number of ranks
auto
rank_and_size() -> std::pair<int, int>
{
    // the rank and size of this process
    int rank, size;

    // delegate to MPI
    MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
    MPI_Comm_size(PETSC_COMM_WORLD, &size);

    // all done
    return { rank, size };
}

// fill a matrix entry by entry, without preallocation
auto
fill(mito::linalg::petsc::matrix_t & A, const lists_t & lists) -> void
{
    // fill matrix entry by entry
    for (std::size_t k = 0; k < std::size(lists.rows); ++k) {
        // skip constrained entries
        if (lists.rows[k] >= 0 && lists.cols[k] >= 0) {
            A.add_value(lists.rows[k], lists.cols[k], lists.vals[k]);
        }
    }

    // assemble the matrix
    A.assemble();

    // all done
    return;
}

// run {f} and return the elapsed wall time, as seen by the slowest rank
template <class F>
auto
timed(F && f) -> double
{
    // the start and end times
    PetscLogDouble t0, t1;

    // synchronize, so all ranks start together
    MPI_Barrier(PETSC_COMM_WORLD);
    PetscTime(&t0);

    // run the function
    f();

    // synchronize, so the time includes the slowest rank
    MPI_Barrier(PETSC_COMM_WORLD);
    PetscTime(&t1);

    // all done
    return t1 - t0;
}

// count the total memory used by all ranks, in GB
auto
total_memory_gb() -> double
{
    // the current resident memory of this process, in bytes
    PetscLogDouble memory;

    // delegate to PETSc
    PetscMemoryGetCurrentUsage(&memory);

    // the sum over all ranks, on every rank
    double total;

    // delegate to MPI
    MPI_Allreduce(&memory, &total, 1, MPI_DOUBLE, MPI_SUM, PETSC_COMM_WORLD);

    // all done
    return total / 1e9;
}

// the COO lists and block segments of this rank's elements, on a structured grid of
// {nx} x {nx} 4-node quadrilaterals; nodes with i == 0 are constrained
auto
grid_lists(index_t nx) -> lists_t
{
    // the equation number of node (i, j); constrained nodes (i == 0) have equation number -1
    auto eq = [nx](index_t i, index_t j) -> index_t {
        if (i == 0 || j == 0) {
            return -1;
        }
        return (i - 1) + (j - 1) * nx;
    };

    // bilinear Laplacian element matrix (times 6), nodes counterclockwise
    const std::vector<scalar_t> Ke = { 4, -1, -2, -1, -1, 4, -1, -2, -2, -1, 4, -1, -1, -2, -1, 4 };

    // this rank's elements: a contiguous band of element rows
    auto [rank, size] = rank_and_size();
    const index_t j0 = nx * rank / size;
    const index_t j1 = nx * (rank + 1) / size;

    // flatten the elements owned by this rank into COO lists
    lists_t lists;

    // reserve space in the lists
    const auto n_entries = static_cast<std::size_t>((j1 - j0) * nx) * 16;
    lists.rows.reserve(n_entries);
    lists.cols.reserve(n_entries);
    lists.vals.reserve(n_entries);

    for (index_t j = j0; j < j1; ++j) {
        for (index_t i = 0; i < nx; ++i) {
            // the equation numbers of the four nodes of this element, counterclockwise
            const std::array<index_t, 4> e = { eq(i, j), eq(i + 1, j), eq(i + 1, j + 1),
                                               eq(i, j + 1) };
            for (std::size_t a = 0; a < 4; ++a) {
                for (std::size_t b = 0; b < 4; ++b) {
                    lists.rows.push_back(e[a]);
                    lists.cols.push_back(e[b]);
                    lists.vals.push_back(Ke[a * 4 + b]);
                }
            }
        }
    }

    // create the segments; all elements are blocks of size 4 thus there is one segment per rank
    lists.segments = { { 4, static_cast<std::size_t>((j1 - j0) * nx) } };

    // all done
    return lists;
}


int
main(int argc, char ** argv)
{
    // initialize PETSc (and MPI)
    auto environment = mito::linalg::petsc::environment();

    // initialize the benchmark library
    benchmark::Initialize(&argc, argv);

    // problem size and number of iterations, settable with -bench_nx and -bench_iterations
    // (through PETSC_OPTIONS)
    PetscInt nx = 300;
    PetscInt iterations = 10;
    PetscOptionsGetInt(nullptr, nullptr, "-bench_nx", &nx, nullptr);
    PetscOptionsGetInt(nullptr, nullptr, "-bench_iterations", &iterations, nullptr);

    // the number of equations: nodes (i, j) with i in [1:nx], j in [0:nx]
    const index_t N = nx * nx;

    // the benchmarks for each format
    for (const auto & [name, format] : formats) {
        // a copy of the format, for capturing in the lambdas
        const auto fmt = format;

        // the name of the benchmark, e.g. "assembly/coo/set_pattern"
        const auto pattern_name = std::string("assembly/") + name + "/set_pattern";

        // register the benchmark, set the number of iterations, and use manual timing
        benchmark::RegisterBenchmark(
            pattern_name.c_str(),
            [nx, N, fmt](benchmark::State & state) {
                for (auto _ : state) {
                    // create the assembly input for this rank, copied from the shared lists
                    auto lists = grid_lists(nx);

                    // create a fresh matrix, with no pattern (not timed)
                    auto A = mito::linalg::petsc::matrix("A", N, fmt);

                    // time the pattern step
                    state.SetIterationTime(timed([&] {
                        A.set_pattern(
                            std::move(lists.rows), std::move(lists.cols),
                            std::move(lists.segments));
                    }));
                }
            })
            ->Iterations(iterations)
            ->UseManualTime()
            ->Unit(benchmark::kMillisecond);

        // the name of the benchmark, e.g. "assembly/coo/first_assembly"
        const auto first_name = std::string("assembly/") + name + "/first_assembly";

        // register the benchmark, set the number of iterations, and use manual timing
        benchmark::RegisterBenchmark(
            first_name.c_str(),
            [nx, N, fmt](benchmark::State & state) {
                for (auto _ : state) {
                    // create the assembly input for this rank, copied from the shared lists
                    auto lists = grid_lists(nx);

                    // create a fresh matrix, with no pattern (not timed)
                    auto A = mito::linalg::petsc::matrix("A", N, fmt);

                    // set the pattern once (not timed)
                    A.set_pattern(
                        std::move(lists.rows), std::move(lists.cols), std::move(lists.segments));

                    // time the first assembly
                    state.SetIterationTime(timed([&] {
                        A.set_values(lists.vals);
                        A.assemble();
                    }));
                }
            })
            ->Iterations(iterations)
            ->UseManualTime()
            ->Unit(benchmark::kMillisecond);

        // the name of the benchmark, e.g. "assembly/coo/repeat_assembly"
        const auto repeat_name = std::string("assembly/") + name + "/repeat_assembly";

        // register the benchmark, set the number of iterations, and use manual timing
        benchmark::RegisterBenchmark(
            repeat_name.c_str(),
            [nx, N, fmt](benchmark::State & state) {
                // create the assembly input for this rank, copied from the shared lists
                auto lists = grid_lists(nx);

                // memory before the matrix exists
                const auto m0 = total_memory_gb();

                // create a matrix assembled once (not timed)
                auto A = mito::linalg::petsc::matrix("A", N, fmt);

                // set the pattern once (not timed)
                A.set_pattern(
                    std::move(lists.rows), std::move(lists.cols), std::move(lists.segments));

                // set the values and assemble once (not timed)
                A.set_values(lists.vals);
                A.assemble();

                // memory after the matrix is assembled
                const auto m1 = total_memory_gb();

                for (auto _ : state) {
                    // time one re-set and re-assembly
                    state.SetIterationTime(timed([&] {
                        A.set_values(lists.vals);
                        A.assemble();
                    }));
                }

                // report the memory as extra columns
                state.counters["set_values_GB"] = m1 - m0;
            })
            ->Iterations(iterations)
            ->UseManualTime()
            ->Unit(benchmark::kMillisecond);
    }

    // register the benchmark, set the number of iterations, and use manual timing
    benchmark::RegisterBenchmark(
        "assembly/add_value/first",
        [nx, N](benchmark::State & state) {
            for (auto _ : state) {
                // create the assembly input for this rank, copied from the shared lists
                auto lists = grid_lists(nx);

                // create a fresh matrix, with no pattern (not timed)
                auto A = mito::linalg::petsc::matrix("A", N);

                // time the first assembly
                state.SetIterationTime(timed([&] { fill(A, lists); }));
            }
        })
        ->Iterations(iterations)
        ->UseManualTime()
        ->Unit(benchmark::kMillisecond);

    // register the benchmark, set the number of iterations, and use manual timing
    benchmark::RegisterBenchmark(
        "assembly/add_value/repeat",
        [nx, N](benchmark::State & state) {
            // create the assembly input for this rank, copied from the shared lists
            auto lists = grid_lists(nx);

            // memory before the matrix exists
            const auto m0 = total_memory_gb();

            // create a matrix
            auto A = mito::linalg::petsc::matrix("A", N);

            // fill A once (not timed)
            fill(A, lists);

            // memory after the matrix is assembled
            const auto m1 = total_memory_gb();

            for (auto _ : state) {
                // time one re-assembly
                state.SetIterationTime(timed([&] {
                    MatZeroEntries(A.get());
                    fill(A, lists);
                }));
            }

            // report the memory as extra column
            state.counters["set_values_GB"] = m1 - m0;
        })
        ->Iterations(iterations)
        ->UseManualTime()
        ->Unit(benchmark::kMillisecond);

    // register a baseline benchmark that does nothing, to measure the overhead of any imported
    // libraries and the benchmark library itself
    benchmark::RegisterBenchmark(
        "assembly/baseline",
        [nx](benchmark::State & state) {
            auto lists = grid_lists(nx);
            for (auto _ : state) {
                benchmark::DoNotOptimize(lists);
                state.SetIterationTime(0.0);
            }
        })
        ->Iterations(iterations)
        ->UseManualTime()
        ->Unit(benchmark::kMillisecond);

    // run the benchmarks
    auto [rank, size] = rank_and_size();

    // only rank 0 reports the benchmark results
    if (rank == 0) {
        benchmark::RunSpecifiedBenchmarks();
    } else {
        NullReporter null_reporter;
        benchmark::RunSpecifiedBenchmarks(&null_reporter);
    }

    // finalize the benchmark library
    benchmark::Shutdown();

    // all done
    return 0;
}

// end of file