// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>

#include <array>
#include <utility>
#include <vector>


namespace {

    // types
    using index_t = mito::linalg::petsc::index_t;
    using scalar_t = mito::linalg::petsc::scalar_t;
    using format_t = mito::linalg::assembly_format;
    using segment_t = mito::linalg::block_segment_t;

    // the assembly formats under test
    constexpr std::array<std::pair<const char *, format_t>, 3> formats = { {
        { "hash", format_t::hash },
        { "coo", format_t::coo },
        { "csr", format_t::csr },
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
    auto rank_and_size() -> std::pair<int, int>
    {
        int rank, size;
        MPI_Comm_rank(PETSC_COMM_WORLD, &rank);
        MPI_Comm_size(PETSC_COMM_WORLD, &size);
        return { rank, size };I
    }

    // a hand-made system with 5 equations: two 2-node and two 3-node elements,
    // one constrained node, a non-symmetric element matrix and an explicit zero
    auto test_elements() -> std::vector<element_t>
    {
        return {
            { { 0, 1 }, { 2, -1, -1, 2 } },
            { { 1, 2 }, { 3, -2, -1, 4 } },
            { { 2, 3, -1 }, { 1, 2, 3, 4, 5, 6, 7, 8, 9 } },
            { { 3, 4, 0 }, { 1, 0, 2, 0, 3, 0, 4, 0, 5 } },
        };
    }

    // flatten the elements owned by this rank (round robin) into COO lists and block segments;
    // the element matrices are scaled by {factor}
    auto flatten(const std::vector<element_t> & elements, scalar_t factor) -> lists_t
    {
        auto [rank, size] = rank_and_size();

        lists_t lists;
        for (std::size_t k = 0; k < std::size(elements); ++k) {
            // skip elements owned by other ranks
            if (static_cast<int>(k % size) != rank) {
                continue;
            }

            const auto & [eqs, Ke] = elements[k];
            const auto n = std::size(eqs);
            for (std::size_t a = 0; a < n; ++a) {
                for (std::size_t b = 0; b < n; ++b) {
                    lists.rows.push_back(eqs[a]);
                    lists.cols.push_back(eqs[b]);
                    lists.vals.push_back(factor * Ke[a * n + b]);
                }
            }

            // extend the last segment if the block size matches, else start a new one
            if (!std::empty(lists.segments) && lists.segments.back().block_size == n) {
                ++lists.segments.back().n_blocks;
            } else {
                lists.segments.push_back({ n, 1 });
            }
        }

        return lists;
    }

    // the reference: the same elements assembled entry by entry with {add_value}
    template <class matrixT>
    auto fill_reference(matrixT & R, const std::vector<element_t> & elements, scalar_t factor)
        -> void
    {
        auto [rank, size] = rank_and_size();

        for (std::size_t k = 0; k < std::size(elements); ++k) {
            if (static_cast<int>(k % size) != rank) {
                continue;
            }

            const auto & [eqs, Ke] = elements[k];
            const auto n = std::size(eqs);
            for (std::size_t a = 0; a < n; ++a) {
                for (std::size_t b = 0; b < n; ++b) {
                    if (eqs[a] >= 0 && eqs[b] >= 0) {
                        R.add_value(eqs[a], eqs[b], factor * Ke[a * n + b]);
                    }
                }
            }
        }

        R.assemble();
    }

    // compare all entries in the rows owned by this rank
    template <class matrixA, class matrixB>
    auto expect_equal(const matrixA & A, const matrixB & B, index_t N) -> void
    {
        index_t rstart, rend;
        MatGetOwnershipRange(A.get(), &rstart, &rend);

        for (index_t i = rstart; i < rend; ++i) {
            for (index_t j = 0; j < N; ++j) {
                EXPECT_NEAR(A.get_value(i, j), B.get_value(i, j), 1e-12)
                    << "entry (" << i << ", " << j << ")";
            }
        }
    }

    // check one entry, on the rank that owns its row
    template <class matrixT>
    auto expect_entry(const matrixT & A, index_t i, index_t j, scalar_t expected) -> void
    {
        index_t rstart, rend;
        MatGetOwnershipRange(A.get(), &rstart, &rend);

        if (rstart <= i && i < rend) {
            EXPECT_NEAR(A.get_value(i, j), expected, 1e-12) << "entry (" << i << ", " << j << ")";
        }
    }

    // run {f} and return the elapsed wall time, as seen by the slowest rank
    template <class F>
    auto timed(F && f) -> PetscLogDouble
    {
        PetscLogDouble t0, t1;
        MPI_Barrier(PETSC_COMM_WORLD);
        PetscTime(&t0);
        f();
        MPI_Barrier(PETSC_COMM_WORLD);
        PetscTime(&t1);
        return t1 - t0;
    }

}    // namespace


// all formats produce the same matrix as entry-by-entry assembly, also on re-assembly
TEST(Math, PETScAssemblyFormats)
{
    const index_t N = 5;
    const auto elements = test_elements();

    for (const auto & [name, format] : formats) {
        SCOPED_TRACE(name);

        // pattern
        auto lists = flatten(elements, 1.0);
        auto A = mito::linalg::petsc::matrix("A", N, format);
        A.set_pattern(lists.rows, lists.cols, lists.segments);

        // first assembly
        A.set_values(lists.vals);
        A.assemble();

        auto R1 = mito::linalg::petsc::matrix("R1", N);
        fill_reference(R1, elements, 1.0);
        expect_equal(A, R1, N);

        // a few entries checked by hand, independently of the reference
        expect_entry(A, 0, 0, 7.0);     // 2 (element 0) + 5 (element 3)
        expect_entry(A, 1, 1, 5.0);     // 2 (element 0) + 3 (element 1)
        expect_entry(A, 1, 2, -2.0);    // element 1, non-symmetric
        expect_entry(A, 2, 1, -1.0);    // element 1, non-symmetric
        expect_entry(A, 0, 3, 4.0);     // element 3
        expect_entry(A, 2, 4, 0.0);     // not in the pattern

        // re-assembly with different values must replace, not accumulate
        auto lists2 = flatten(elements, 2.0);
        A.set_values(lists2.vals);
        A.assemble();

        auto R2 = mito::linalg::petsc::matrix("R2", N);
        fill_reference(R2, elements, 2.0);
        expect_equal(A, R2, N);
    }

    // all done
    return;
}


// Keff = K + M / dt through a shared pattern, for each format of K and M
TEST(Math, PETScEffectiveMatrix)
{
    const index_t N = 5;
    const auto elements = test_elements();

    for (const auto & [name, format] : formats) {
        SCOPED_TRACE(name);

        // K = elements, M = 2 * elements
        auto listsK = flatten(elements, 1.0);
        auto listsM = flatten(elements, 2.0);

        auto K = mito::linalg::petsc::matrix("K", N, format);
        K.set_pattern(listsK.rows, listsK.cols, listsK.segments);
        K.set_values(listsK.vals);
        K.assemble();

        auto M = mito::linalg::petsc::matrix("M", N, format);
        M.set_pattern(listsM.rows, listsM.cols, listsM.segments);
        M.set_values(listsM.vals);
        M.assemble();

        // Keff = K + M / 2 = 2 * elements
        auto Keff = mito::linalg::petsc::matrix("Keff", N);
        Keff.copy_structure(K, mito::linalg::petsc::duplicate_policy::share_pattern);
        Keff.copy_values(K, mito::linalg::petsc::sparsity_pattern::same);
        Keff.add(0.5, M, mito::linalg::petsc::sparsity_pattern::same);

        auto R = mito::linalg::petsc::matrix("R", N);
        fill_reference(R, elements, 2.0);
        expect_equal(Keff, R, N);
    }

    // all done
    return;
}


// relative performance of the formats on a structured grid of 4-node quadrilaterals;
// disabled by default, run with --gtest_also_run_disabled_tests
TEST(Math, DISABLED_PETScAssemblyBenchmark)
{
    // problem size and number of re-assemblies, settable with -bench_nx and -bench_repeats
    PetscInt nx = 300;
    PetscInt repeats = 10;
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
    return;
}


int
main(int argc, char ** argv)
{
    // initialize PETSc
    auto environment = mito::linalg::petsc::environment();

    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    // all done
    return result;
}


// end of file