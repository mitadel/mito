// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include "utilities/petsc_assembly.h"


using namespace mito::tests::linalg::petsc;

// a duplicate keeps the assembly data of its source, so set_values works on it
auto
check_duplicate_set_values(format_t format) -> void
{
    // the elements owned by this rank, and the number of equations
    const auto local = local_elements(test_elements());
    const index_t N = n_equations(local);

    // K with its pattern and values
    auto listsK = flatten(local, 1.0);
    auto K = mito::linalg::petsc::matrix_t("K", N, format);
    K.set_pattern(listsK.rows, listsK.cols, listsK.segments);
    K.set_values(listsK.vals);
    K.assemble();

    // a duplicate of K, refilled through set_values with different values
    auto D = mito::linalg::petsc::matrix_t("D", N, format);
    D.duplicate(K, mito::linalg::petsc::duplicate_policy::copy_pattern);
    auto listsD = flatten(local, 3.0);
    D.set_values(listsD.vals);
    D.assemble();

    // the duplicate holds the new values
    auto RD = mito::linalg::petsc::matrix_t("RD", N);
    fill_reference(RD, local, 3.0);
    expect_equal(D, RD, N);

    // K is unchanged (catches a duplicate that writes into its source)
    auto RK = mito::linalg::petsc::matrix_t("RK", N);
    fill_reference(RK, local, 1.0);
    expect_equal(K, RK, N);
}


TEST(Math, PETScDuplicateSetValuesCSR)
{
    check_duplicate_set_values(format_t::csr);
}

TEST(Math, PETScDuplicateSetValuesHash)
{
    check_duplicate_set_values(format_t::hash);
}

TEST(Math, PETScDuplicateSetValuesCOO)
{
    check_duplicate_set_values(format_t::coo);
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