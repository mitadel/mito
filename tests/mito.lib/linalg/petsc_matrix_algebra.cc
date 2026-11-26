// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>

#include <array>
#include <vector>


// the size of the square matrix
constexpr int N = 3;


// fill {A} with the matrix
//   [ 1  2  0 ]
//   [ 0  3  4 ]
//   [ 5  0  6 ]
// which is not symmetric, so a product with the transpose would be caught; then assemble it
auto
fill_test_matrix(auto & A) -> void
{
    // set matrix entries
    A.insert_value(0, 0, 1.0);
    A.insert_value(0, 1, 2.0);
    A.insert_value(1, 1, 3.0);
    A.insert_value(1, 2, 4.0);
    A.insert_value(2, 0, 5.0);
    A.insert_value(2, 2, 6.0);

    // assemble the matrix
    A.assemble();

    // all done
    return;
}

// set the entries of {vector} to {values} and assemble the vector
auto
fill_vector(auto & vector, const std::array<double, N> & values) -> void
{
    // set vector entries
    for (int i = 0; i < N; i++) {
        vector.insert_value(i, values[i]);
    }

    // assemble the vector
    vector.assemble();

    // all done
    return;
}

// copy the entries of {vector} into a {std::vector}
auto
to_std_vector(const auto & vector) -> std::vector<double>
{
    // read the vector
    auto result = std::vector<double>(N);
    vector.get_vector(result);

    // all done
    return result;
}


TEST(Math, PETScMatrixSize)
{
    // instantiate a PETSc matrix of size {N}x{N}
    auto A = mito::linalg::petsc::matrix("A", N);

    // check the returned size
    EXPECT_EQ(A.size(), N);

    // check the size written to the output argument
    mito::linalg::petsc::index_t size = -1;
    A.size(size);
    EXPECT_EQ(size, N);

    // all done
    return;
}


TEST(Math, PETScMatrixScale)
{
    // instantiate and fill the test matrix
    auto A = mito::linalg::petsc::matrix("A", N);
    fill_test_matrix(A);

    // A <- -0.5 * A
    A.scale(-0.5);

    // check the solution (including the entries outside the pattern)
    EXPECT_DOUBLE_EQ(A.get_value(0, 0), -0.5);
    EXPECT_DOUBLE_EQ(A.get_value(0, 1), -1.0);
    EXPECT_DOUBLE_EQ(A.get_value(0, 2), 0.0);
    EXPECT_DOUBLE_EQ(A.get_value(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(A.get_value(1, 1), -1.5);
    EXPECT_DOUBLE_EQ(A.get_value(1, 2), -2.0);
    EXPECT_DOUBLE_EQ(A.get_value(2, 0), -2.5);
    EXPECT_DOUBLE_EQ(A.get_value(2, 1), 0.0);
    EXPECT_DOUBLE_EQ(A.get_value(2, 2), -3.0);

    // all done
    return;
}


// mult_vector: y <- A * x (MatMult)
TEST(Math, PETScMatrixMultVector)
{
    // instantiate and fill the test matrix
    auto A = mito::linalg::petsc::matrix("A", N);
    fill_test_matrix(A);

    // x = (1, 2, 3)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, { 1.0, 2.0, 3.0 });

    // y <- A * x
    auto y = mito::linalg::petsc::vector("y", N);
    A.mult_vector(x, y);

    // check the solution (A^T * x would give (16, 8, 26))
    auto result = to_std_vector(y);
    EXPECT_DOUBLE_EQ(result[0], 5.0);
    EXPECT_DOUBLE_EQ(result[1], 18.0);
    EXPECT_DOUBLE_EQ(result[2], 23.0);

    // {x} is unchanged
    auto x_result = to_std_vector(x);
    EXPECT_DOUBLE_EQ(x_result[0], 1.0);
    EXPECT_DOUBLE_EQ(x_result[1], 2.0);
    EXPECT_DOUBLE_EQ(x_result[2], 3.0);

    // all done
    return;
}


// mult_add_vector: z <- A * x + y (MatMultAdd)
TEST(Math, PETScMatrixMultAddVector)
{
    // instantiate and fill the test matrix
    auto A = mito::linalg::petsc::matrix("A", N);
    fill_test_matrix(A);

    // x = (1, 2, 3)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, { 1.0, 2.0, 3.0 });

    // y = (10, 20, 30)
    auto y = mito::linalg::petsc::vector("y", N);
    fill_vector(y, { 10.0, 20.0, 30.0 });

    // z <- A * x + y
    auto z = mito::linalg::petsc::vector("z", N);
    A.mult_add_vector(x, y, z);

    // check the solution
    auto result = to_std_vector(z);
    EXPECT_DOUBLE_EQ(result[0], 15.0);
    EXPECT_DOUBLE_EQ(result[1], 38.0);
    EXPECT_DOUBLE_EQ(result[2], 53.0);

    // {y} is unchanged
    auto y_result = to_std_vector(y);
    EXPECT_DOUBLE_EQ(y_result[0], 10.0);
    EXPECT_DOUBLE_EQ(y_result[1], 20.0);
    EXPECT_DOUBLE_EQ(y_result[2], 30.0);

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