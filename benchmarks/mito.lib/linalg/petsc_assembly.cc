// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// get the benchmark library
#include <benchmark/benchmark.h>
// get the mito library
#include <mito.h>
// include the shared benchmark support
#include "utilities/petsc_assembly.h"


using namespace mito::benchmarks::linalg::petsc;


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
    const index_t N = n_equations(nx);

    // the benchmarks for each format
    for (const auto & [name, format] : formats) {
        // a copy of the format, for capturing in the lambdas
        const auto fmt = format;

        // the name of the benchmark, e.g. "assembly/coo/set_pattern"
        const auto pattern_name = std::string("assembly/") + name + "/set_pattern";

        // register the benchmark, set the number of iterations, and use manual timing
        register_timed(pattern_name, iterations, [nx, N, fmt](benchmark::State & state) {
            for (auto _ : state) {
                // create the assembly input for this rank, copied from the shared lists
                auto lists = grid_lists(nx, stiffness_element);

                // create a fresh matrix, with no pattern (not timed)
                auto A = mito::linalg::petsc::matrix("A", N, fmt);

                // time the pattern step
                state.SetIterationTime(timed([&] {
                    A.set_pattern(
                        std::move(lists.rows), std::move(lists.cols), std::move(lists.segments));
                }));
            }
        });

        // the name of the benchmark, e.g. "assembly/coo/first_assembly"
        const auto first_name = std::string("assembly/") + name + "/first_assembly";

        // register the benchmark, set the number of iterations, and use manual timing
        register_timed(first_name, iterations, [nx, N, fmt](benchmark::State & state) {
            for (auto _ : state) {
                // create the assembly input for this rank, copied from the shared lists
                auto lists = grid_lists(nx, stiffness_element);

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
        });

        // the name of the benchmark, e.g. "assembly/coo/repeat_assembly"
        const auto repeat_name = std::string("assembly/") + name + "/repeat_assembly";

        // register the benchmark, set the number of iterations, and use manual timing
        register_timed(repeat_name, iterations, [nx, N, fmt](benchmark::State & state) {
            for (auto _ : state) {
                // create the assembly input for this rank, copied from the shared lists
                auto lists = grid_lists(nx, stiffness_element);

                // memory before the matrix exists
                const auto m0 = total_memory_gb();

                // create a fresh matrix, with no pattern (not timed)
                auto A = mito::linalg::petsc::matrix("A", N, fmt);

                // set the pattern once (not timed)
                A.set_pattern(
                    std::move(lists.rows), std::move(lists.cols), std::move(lists.segments));

                // set the values and assemble once (not timed)
                A.set_values(lists.vals);
                A.assemble();

                // time one re-set and re-assembly
                state.SetIterationTime(timed([&] {
                    A.set_values(lists.vals);
                    A.assemble();
                }));

                // memory after the matrix is created
                const auto m1 = total_memory_gb();

                // report the memory as extra columns
                state.counters["set_values_GB"] = m1 - m0;
            }
        });
    }

    // register the benchmark, set the number of iterations, and use manual timing
    register_timed("assembly/add_value", iterations, [nx](benchmark::State & state) {
        for (auto _ : state) {
            // create the assembly input for this rank, copied from the shared lists
            auto lists = grid_lists(nx, stiffness_element);

            // create a fresh matrix, with no pattern (not timed)
            auto A = mito::linalg::petsc::matrix("A", n_equations(nx));

            // time the first assembly
            state.SetIterationTime(timed([&] { fill(A, lists); }));
        }
    });

    // register the benchmark, set the number of iterations, and use manual timing
    register_timed("assembly/add_value/repeat", iterations, [nx](benchmark::State & state) {
        for (auto _ : state) {
            // create the assembly input for this rank, copied from the shared lists
            auto lists = grid_lists(nx, stiffness_element);

            // memory before the matrix exists
            const auto m0 = total_memory_gb();

            // create a fresh matrix, with no pattern (not timed)
            auto A = mito::linalg::petsc::matrix("A", n_equations(nx));

            // fill A once (not timed)
            fill(A, lists);

            // time one re-assembly
            state.SetIterationTime(timed([&] {
                MatZeroEntries(A.get());
                fill(A, lists);
            }));

            // memory after the matrix is created
            const auto m1 = total_memory_gb();

            // report the memory as extra column
            state.counters["set_values_GB"] = m1 - m0;
        }
    });

    // register a baseline benchmark that does nothing, to measure the overhead of any imported
    // libraries and the benchmark library itself
    register_timed("assembly/baseline", iterations, [nx](benchmark::State & state) {
        for (auto _ : state) {
            // create the assembly input for this rank, copied from the shared lists
            auto lists = grid_lists(nx, stiffness_element);

            // time nothing
            state.SetIterationTime(timed([&] { benchmark::DoNotOptimize(lists); }));
        }
    });

    // run the benchmarks
    run_benchmarks();

    // finalize the benchmark library
    benchmark::Shutdown();

    // all done
    return 0;
}

// end of file