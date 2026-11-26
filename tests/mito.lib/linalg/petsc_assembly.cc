// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include "utilities/petsc_assembly.h"


using namespace mito::tests::linalg::petsc;


// all formats produce the same matrix as entry-by-entry assembly, also on re-assembly
TEST(Math, PETScAssemblyFormats)
{
    // the elements to be assembled, in global order
    const auto elements = test_elements();

    // the elements owned by this rank
    const auto local = local_elements(elements);

    // the number of equations in the system
    const index_t N = n_equations(elements);

    for (const auto & [name, format] : formats) {
        // set a trace for this format, so any failure will report the format name
        SCOPED_TRACE(name);

        // flatten the elements into COO lists and block segments, scaled by 1.0
        auto lists = flatten(elements, 1.0);

        // create a matrix of the given format
        auto A = mito::linalg::petsc::matrix("A", N, format);

        // set the pattern from the flattened lists
        A.set_pattern(lists.rows, lists.cols, lists.segments);

        // set the values
        A.set_values(lists.vals);

        // assemble the matrix
        A.assemble();

        // create the reference matrix
        auto R1 = mito::linalg::petsc::matrix("R1", N);

        // fill the reference matrix with the same elements, scaled by 1.0
        fill_reference(R1, elements, 1.0);

        // compare the assembled matrix with the reference
        expect_equal(A, R1, N);

        // a few entries checked by hand, independently of the reference
        expect_entry(A, 0, 0, 7.0);     // 2 (element 0) + 5 (element 3)
        expect_entry(A, 1, 1, 5.0);     // 2 (element 0) + 3 (element 1)
        expect_entry(A, 1, 2, -2.0);    // element 1, non-symmetric
        expect_entry(A, 2, 1, -1.0);    // element 1, non-symmetric
        expect_entry(A, 0, 3, 4.0);     // element 3
        expect_entry(A, 2, 4, 0.0);     // not in the pattern

        // re-assembly with different values must replace, not accumulate
        // flatten the elements into COO lists and block segments, scaled by 2.0
        auto lists2 = flatten(elements, 2.0);

        // set the values
        A.set_values(lists2.vals);

        // assemble the matrix
        A.assemble();

        // create the reference matrix
        auto R2 = mito::linalg::petsc::matrix("R2", N);

        // fill the reference matrix with the same elements, scaled by 2.0
        fill_reference(R2, elements, 2.0);

        // compare the re-assembled matrix with the reference
        expect_equal(A, R2, N);
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

    // initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // run all tests
    auto result = RUN_ALL_TESTS();

    // all done
    return result;
}


// end of file