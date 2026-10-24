// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Math, PETScMatrix)
{
    // the size of the square matrix
    int N = 3;

    // instantiate a PETSc matrix of size {N}x{N}
    auto A = mito::linalg::petsc::matrix("A", N);
    auto B = mito::linalg::petsc::matrix("B", N);

    // set matrix A & B entries
    A.insert_value(0, 0, 1.0);
    A.insert_value(0, 1, 1.0);
    A.insert_value(1, 0, 3.0);
    A.insert_value(1, 1, 4.0);
    A.insert_value(2, 0, 5.0);
    A.insert_value(2, 1, 7.0);

    B.copy_values(A, mito::linalg::petsc::sparsity_pattern::different);

    // check the solution
    EXPECT_DOUBLE_EQ(B.get_value(0, 0), A.get_value(0, 0));
    EXPECT_DOUBLE_EQ(B.get_value(0, 1), A.get_value(0, 1));
    EXPECT_DOUBLE_EQ(B.get_value(0, 2), A.get_value(0, 2));
    EXPECT_DOUBLE_EQ(B.get_value(1, 0), A.get_value(1, 0));
    EXPECT_DOUBLE_EQ(B.get_value(1, 1), A.get_value(1, 1));
    EXPECT_DOUBLE_EQ(B.get_value(1, 2), A.get_value(1, 2));
    EXPECT_DOUBLE_EQ(B.get_value(2, 0), A.get_value(2, 0));
    EXPECT_DOUBLE_EQ(B.get_value(2, 1), A.get_value(2, 1));
    EXPECT_DOUBLE_EQ(B.get_value(2, 2), A.get_value(2, 2));

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
