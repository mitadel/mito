// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include "utilities/petsc_assembly.h"


using namespace mito::tests::linalg::petsc;


// Keff = K + M / dt through a shared pattern, for each format of K and M
TEST(Math, PETScEffectiveMatrix)
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

        // flatten the elements into COO lists and block segments, scaled by 1.0 and 2.0
        auto listsK = flatten(elements, 1.0);
        auto listsM = flatten(elements, 2.0);

        // create the matrix
        auto K = mito::linalg::petsc::matrix("K", N, format);

        // set the pattern of K
        K.set_pattern(listsK.rows, listsK.cols, listsK.segments);

        // set the values of K
        K.set_values(listsK.vals);

        // assemble the matrix K
        K.assemble();

        // create the matrix
        auto M = mito::linalg::petsc::matrix("M", N, format);

        // set the pattern of M
        M.set_pattern(listsM.rows, listsM.cols, listsM.segments);

        // set the values of M
        M.set_values(listsM.vals);

        // assemble the matrix M
        M.assemble();

        // Keff = K + M / 2 = K + (2*K)/2 = 2 * K
        auto Keff = mito::linalg::petsc::matrix("Keff", N);

        // copy the structure of K into Keff, sharing the sparsity pattern
        Keff.copy_structure(K, mito::linalg::petsc::duplicate_policy::share_pattern);

        // copy the values of K into Keff, sharing the sparsity pattern
        Keff.copy_values(K, mito::linalg::petsc::sparsity_pattern::same);

        // add M / 2 into Keff, sharing the sparsity pattern
        Keff.add(0.5, M, mito::linalg::petsc::sparsity_pattern::same);

        // create the reference matrix
        auto R = mito::linalg::petsc::matrix("R", N);

        // fill the reference matrix with the same elements, scaled by 2.0
        fill_reference(R, elements, 2.0);

        // compare the effective matrix with the reference
        expect_equal(Keff, R, N);
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