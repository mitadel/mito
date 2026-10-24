// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

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

// an element: its equation numbers (-1 = constrained) and its row-major element matrix
struct element_t {
    std::vector<index_t> eqs;
    std::vector<scalar_t> Ke;
};

// the flat assembly input of one rank
struct lists_t {
    std::vector<index_t> rows;
    std::vector<index_t> cols;
    std::vector<scalar_t> vals;
    std::vector<segment_t> segments;
};

// the rank of this process and the number of ranks
inline auto
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

// run {f} and return the elapsed wall time, as seen by the slowest rank
template <class F>
inline auto
timed(F && f) -> PetscLogDouble
{
    // the elapsed wall time, as seen by the slowest rank
    PetscLogDouble t0, t1;

    // delegate to PETSc and MPI
    MPI_Barrier(PETSC_COMM_WORLD);
    PetscTime(&t0);

    // run the function
    f();

    // delegate to PETSc and MPI
    MPI_Barrier(PETSC_COMM_WORLD);
    PetscTime(&t1);

    // all done
    return t1 - t0;
}

// relative performance of the formats on a structured grid of 4-node quadrilaterals
int
main()
{
    // initialize PETSc
    auto environment = mito::linalg::petsc::environment();

    // problem size and number of re-assemblies
    PetscInt nx = 300;
    PetscInt repeats = 10;

    // set from command line options, if present
    PetscOptionsGetInt(nullptr, nullptr, "-bench_nx", &nx, nullptr);
    PetscOptionsGetInt(nullptr, nullptr, "-bench_repeats", &repeats, nullptr);

    // nodes (i, j) with i, j in 0..nx; nodes with i == 0 are constrained
    const index_t N = nx * (nx + 1);
    auto eq = [nx](index_t i, index_t j) -> index_t {
        return i == 0 ? -1 : (i - 1) + j * nx;
    };

    // bilinear Laplacian element matrix (times 6), nodes counterclockwise
    const std::vector<scalar_t> Ke = { 4, -1, -2, -1, -1, 4, -1, -2, -2, -1, 4, -1, -1, -2, -1, 4 };

    // this rank's elements: a contiguous band of element rows
    auto [rank, size] = rank_and_size();
    const index_t j0 = nx * rank / size;
    const index_t j1 = nx * (rank + 1) / size;

    lists_t lists;
    for (index_t j = j0; j < j1; ++j) {
        for (index_t i = 0; i < nx; ++i) {
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
    lists.segments = { { 4, static_cast<std::size_t>((j1 - j0) * nx) } };

    PetscPrintf(
        PETSC_COMM_WORLD, "%" PetscInt_FMT " equations, %d ranks, %" PetscInt_FMT " repeats\n", N,
        size, repeats);
    PetscPrintf(
        PETSC_COMM_WORLD, "%-10s %14s %14s %14s\n", "format", "pattern [s]", "first [s]",
        "repeat [s]");

    // the new interface, for each format
    for (const auto & [name, format] : formats) {
        auto A = mito::linalg::petsc::matrix(name, N, format);

        // copy the lists outside the timed region, so the pattern step can take them over
        auto rows = lists.rows;
        auto cols = lists.cols;
        auto segments = lists.segments;

        const auto t_pattern =
            timed([&] { A.set_pattern(std::move(rows), std::move(cols), std::move(segments)); });
        const auto t_first = timed([&] {
            A.set_values(lists.vals);
            A.assemble();
        });
        const auto t_repeat = timed([&] {
                                  for (PetscInt r = 0; r < repeats; ++r) {
                                      A.set_values(lists.vals);
                                      A.assemble();
                                  }
                              })
                            / repeats;

        PetscPrintf(
            PETSC_COMM_WORLD, "%-10s %14.4f %14.4f %14.4f\n", name, t_pattern, t_first, t_repeat);
    }

    // the old approach: entry by entry, without preallocation
    {
        auto A = mito::linalg::petsc::matrix("add_value", N);
        auto fill = [&] {
            for (std::size_t k = 0; k < std::size(lists.rows); ++k) {
                if (lists.rows[k] >= 0 && lists.cols[k] >= 0) {
                    A.add_value(lists.rows[k], lists.cols[k], lists.vals[k]);
                }
            }
            A.assemble();
        };

        const auto t_first = timed(fill);
        const auto t_repeat = timed([&] {
                                  for (PetscInt r = 0; r < repeats; ++r) {
                                      MatZeroEntries(A.get());
                                      fill();
                                  }
                              })
                            / repeats;

        PetscPrintf(
            PETSC_COMM_WORLD, "%-10s %14s %14.4f %14.4f\n", "add_value", "-", t_first, t_repeat);
    }

    // all done
    return 0;
}
