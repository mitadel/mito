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

    B.insert_value(0, 1, 2.0);
    B.insert_value(1, 0, 1.0);
    B.insert_value(1, 1, -2.0);
    B.insert_value(1, 2, -5.0);
    B.insert_value(2, 0, 3.5);
    B.insert_value(2, 1, 2.5);

    // assemble the matrices
    A.assemble();
    B.assemble();  

    auto C = mito::linalg::petsc::Matrix ("C", A);
    C.add ( 1.0/10.0, B, mito::linalg::petsc::sparsity_pattern::different);  

    // check the solution
    EXPECT_DOUBLE_EQ(C.get_value(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(C.get_value(0, 1), 1.2);
    EXPECT_DOUBLE_EQ(C.get_value(0, 2), 0.0);
    EXPECT_DOUBLE_EQ(C.get_value(1, 0), 3.1);
    EXPECT_DOUBLE_EQ(C.get_value(1, 1), 3.8);
    EXPECT_DOUBLE_EQ(C.get_value(1, 2), -0.5);
    EXPECT_DOUBLE_EQ(C.get_value(2, 0), 5.35);
    EXPECT_DOUBLE_EQ(C.get_value(2, 1), 7.25);
    EXPECT_DOUBLE_EQ(C.get_value(2, 2), 0.0);

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
