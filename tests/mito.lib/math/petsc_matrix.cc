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
    auto matrix = mito::math_backend::petsc::matrix("mymatrix");

    // create the matrix and allocate the memory
    matrix.create(N);

    // set vector entries
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix.add_value(i, j, double((i + 1) * (j + 1)));
            matrix.add_value(i, j, double(i - j));
        }
    }

    // flush assemble the matrix
    matrix.assembleFlush();

    // set one entry to a specific value
    matrix.insert_value(0, 2, 5.0);

    // assemble the matrix
    matrix.assemble();

    // check the solution
    EXPECT_DOUBLE_EQ(matrix.get_value(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(0, 1), 1.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(0, 2), 5.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(1, 0), 3.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(1, 1), 4.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(1, 2), 5.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(2, 0), 5.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(2, 1), 7.0);
    EXPECT_DOUBLE_EQ(matrix.get_value(2, 2), 9.0);

    // destroy the matrix and free the memory
    matrix.destroy();

    // all done
    return;
}


int
main(int argc, char ** argv)
{
    // initialize PETSc
    mito::petsc::initialize();

    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    // finalize PETSc
    mito::petsc::finalize();

    // all done
    return result;
}


// end of file
