// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once

// get the benchmark library
#include <benchmark/benchmark.h>
// get the mito library
#include <mito.h>


// shared support for the PETSc benchmarks: a structured grid of 4-node quadrilaterals, MPI-safe
// timing and reporting
namespace mito::benchmarks::linalg::petsc {

    // types
    using index_t = mito::linalg::petsc::index_t;
    using scalar_t = mito::linalg::petsc::scalar_t;
    using format_t = mito::linalg::assembly_format;
    using segment_t = mito::linalg::block_segment_t;

    // an element matrix of a 4-node quadrilateral, row-major
    using element_matrix_t = std::array<scalar_t, 16>;

    // the assembly formats
    inline const std::array<std::pair<std::string, format_t>, 3> formats = { {
        { "coo", format_t::coo },
        { "csr", format_t::csr },
        { "hash", format_t::hash },
    } };

    // bilinear Laplacian (stiffness) element matrix on the unit square
    inline constexpr element_matrix_t stiffness_element = { 4,  -1, -2, -1, -1, 4,  -1, -2,
                                                            -2, -1, 4,  -1, -1, -2, -1, 4 };

    // bilinear consistent mass element matrix on the unit square
    inline constexpr element_matrix_t mass_element = { 4, 2, 1, 2, 2, 4, 2, 1,
                                                       1, 2, 4, 2, 2, 1, 2, 4 };

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
    inline auto rank_and_size() -> std::pair<int, int>
    {
        // the rank and size of this process
        int rank, size;

        // delegate to MPI
        MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
        MPI_Comm_size(PETSC_COMM_WORLD, &size);

        // all done
        return { rank, size };
    }

    // the problem size and number of iterations, settable with -bench_nx and -bench_iterations
    // (through PETSC_OPTIONS)
    inline auto options() -> std::pair<index_t, PetscInt>
    {
        // the defaults
        PetscInt nx = 300;
        PetscInt iterations = 10;

        // override from the options database, if present
        PetscOptionsGetInt(nullptr, nullptr, "-bench_nx", &nx, nullptr);
        PetscOptionsGetInt(nullptr, nullptr, "-bench_iterations", &iterations, nullptr);

        // all done
        return { nx, iterations };
    }

    // run {f} and return the elapsed wall time, as seen by the slowest rank
    template <class F>
    auto timed(F && f) -> double
    {
        // the start and end times
        PetscLogDouble t0, t1;

        // synchronize
        MPI_Barrier(PETSC_COMM_WORLD);

        // delegate to PETSc
        PetscTime(&t0);

        // run the function
        f();

        // synchronize
        MPI_Barrier(PETSC_COMM_WORLD);

        // delegate to PETSc
        PetscTime(&t1);

        // all done
        return t1 - t0;
    }

    // the number of equations of the grid
    inline auto n_equations(index_t nx) -> index_t
    {
        // return the number of nodes with i, j in [1:nx], which is nx * nx
        return nx * nx;
    }

    // the equation number of node (i, j); nodes on the left (i == 0) and bottom (j == 0) edges
    // are constrained and have equation number -1
    inline auto equation(index_t nx, index_t i, index_t j) -> index_t
    {
        if (i == 0 || j == 0) {
            return -1;
        }
        return (i - 1) + (j - 1) * nx;
    }

    // the equation numbers of the four nodes of element (i, j), counterclockwise
    inline auto element_equations(index_t nx, index_t i, index_t j) -> std::array<index_t, 4>
    {
        return { equation(nx, i, j), equation(nx, i + 1, j), equation(nx, i + 1, j + 1),
                 equation(nx, i, j + 1) };
    }

    // this rank's elements: a contiguous band of element rows [j0, j1)
    inline auto element_rows(index_t nx) -> std::pair<index_t, index_t>
    {
        auto [rank, size] = rank_and_size();
        return { nx * rank / size, nx * (rank + 1) / size };
    }

    // the COO lists of this rank's elements, all with element matrix {Ke}; the entries are
    // ordered element by element, so lists built with different {Ke} have the same entry order
    inline auto grid_lists(index_t nx, const element_matrix_t & Ke) -> lists_t
    {
        // this rank's band of element rows and its number of elements
        auto [j0, j1] = element_rows(nx);
        const auto n_elements = static_cast<std::size_t>((j1 - j0) * nx);

        // reserve the final sizes: 16 entries per element
        lists_t lists;
        lists.rows.reserve(16 * n_elements);
        lists.cols.reserve(16 * n_elements);
        lists.vals.reserve(16 * n_elements);

        // flatten the elements
        for (index_t j = j0; j < j1; ++j) {
            for (index_t i = 0; i < nx; ++i) {
                const auto e = element_equations(nx, i, j);
                for (std::size_t a = 0; a < 4; ++a) {
                    for (std::size_t b = 0; b < 4; ++b) {
                        lists.rows.push_back(e[a]);
                        lists.cols.push_back(e[b]);
                        lists.vals.push_back(Ke[a * 4 + b]);
                    }
                }
            }
        }

        // one segment: all elements are blocks of size 4
        lists.segments = { { 4, n_elements } };

        // all done
        return lists;
    }

    // set the pattern of {A} from {lists}, then assemble it with the values of {lists}
    template <class matrixT>
    auto assemble_from(matrixT & A, lists_t lists) -> void
    {
        // set the pattern once
        A.set_pattern(std::move(lists.rows), std::move(lists.cols), std::move(lists.segments));

        // set the values and assemble once
        A.set_values(lists.vals);

        // assemble the matrix
        A.assemble();

        // all done
        return;
    }

    // register a benchmark with a fixed number of iterations and manually measured times; the
    // fixed count makes all ranks make the same collective calls
    template <class F>
    auto register_timed(const std::string & name, PetscInt iterations, F && f) -> void
    {
        // register the benchmark with the given name and function, and set the number of iterations
        benchmark::RegisterBenchmark(name.c_str(), std::forward<F>(f))
            ->Iterations(iterations)
            ->UseManualTime()
            ->Unit(benchmark::kMillisecond);

        // all done
        return;
    }

    // run the registered benchmarks
    inline auto run_benchmarks() -> void
    {
        // get the rank and size
        auto [rank, size] = rank_and_size();

        // only rank 0 reports the benchmark results
        if (rank == 0) {
            benchmark::RunSpecifiedBenchmarks();
        } else {
            NullReporter null_reporter;
            benchmark::RunSpecifiedBenchmarks(&null_reporter);
        }

        // all done
        return;
    }

}    // namespace mito


// end of file