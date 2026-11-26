// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>

#include <array>
#include <vector>


// the size of the vectors in this test
constexpr int N = 5;


// set entry {i} of {vector} to {f(i)} and assemble the vector
auto
fill_vector(auto & vector, auto && f) -> void
{
    // set vector entries
    for (int i = 0; i < N; i++) {
        vector.insert_value(i, f(i));
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


TEST(Math, PETScVectorSize)
{
    // instantiate a PETSc vector of size {N}
    auto x = mito::linalg::petsc::vector("x", N);

    // check the returned size
    EXPECT_EQ(x.size(), N);

    // check the size written to the output argument
    mito::linalg::petsc::index_t size = -1;
    x.size(size);
    EXPECT_EQ(size, N);

    // all done
    return;
}


TEST(Math, PETScVectorZero)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // zero the vector
    x.zero();

    // check the solution
    auto result = to_std_vector(x);
    for (int i = 0; i < N; i++) {
        EXPECT_DOUBLE_EQ(result[i], 0.0) << "entry " << i;
    }

    // all done
    return;
}


TEST(Math, PETScVectorScale)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // x <- -2 * x
    x.scale(-2.0);

    // check the solution
    auto result = to_std_vector(x);
    EXPECT_DOUBLE_EQ(result[0], -2.0);
    EXPECT_DOUBLE_EQ(result[1], -4.0);
    EXPECT_DOUBLE_EQ(result[2], -6.0);
    EXPECT_DOUBLE_EQ(result[3], -8.0);
    EXPECT_DOUBLE_EQ(result[4], -10.0);

    // all done
    return;
}


TEST(Math, PETScVectorCopyValues)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // y <- x
    auto y = mito::linalg::petsc::vector("y", N);
    y.copy_values(x);

    // modify {x} afterwards (catches a copy that aliases its source)
    x.insert_value(0, 100.0);
    x.assemble();

    // {y} holds the values of {x} at the time of the copy
    auto result = to_std_vector(y);
    EXPECT_DOUBLE_EQ(result[0], 1.0);
    EXPECT_DOUBLE_EQ(result[1], 2.0);
    EXPECT_DOUBLE_EQ(result[2], 3.0);
    EXPECT_DOUBLE_EQ(result[3], 4.0);
    EXPECT_DOUBLE_EQ(result[4], 5.0);

    // all done
    return;
}


// add: y <- y + alpha * x (VecAXPY)
TEST(Math, PETScVectorAdd)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // y = (10, 10, 10, 10, 10)
    auto y = mito::linalg::petsc::vector("y", N);
    fill_vector(y, [](int) { return 10.0; });

    // y <- y + 0.5 * x
    y.add(0.5, x);

    // check the solution
    auto result = to_std_vector(y);
    EXPECT_DOUBLE_EQ(result[0], 10.5);
    EXPECT_DOUBLE_EQ(result[1], 11.0);
    EXPECT_DOUBLE_EQ(result[2], 11.5);
    EXPECT_DOUBLE_EQ(result[3], 12.0);
    EXPECT_DOUBLE_EQ(result[4], 12.5);

    // {x} is unchanged
    auto x_result = to_std_vector(x);
    for (int i = 0; i < N; i++) {
        EXPECT_DOUBLE_EQ(x_result[i], double(i + 1)) << "entry " << i;
    }

    // all done
    return;
}


// scale_and_add: y <- x + alpha * y (VecAYPX); same inputs as {PETScVectorAdd}, so that
// swapping AXPY and AYPX would make one of the two tests fail
TEST(Math, PETScVectorScaleAndAdd)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // y = (10, 10, 10, 10, 10)
    auto y = mito::linalg::petsc::vector("y", N);
    fill_vector(y, [](int) { return 10.0; });

    // y <- x + 0.5 * y
    y.scale_and_add(0.5, x);

    // check the solution
    auto result = to_std_vector(y);
    EXPECT_DOUBLE_EQ(result[0], 6.0);
    EXPECT_DOUBLE_EQ(result[1], 7.0);
    EXPECT_DOUBLE_EQ(result[2], 8.0);
    EXPECT_DOUBLE_EQ(result[3], 9.0);
    EXPECT_DOUBLE_EQ(result[4], 10.0);

    // all done
    return;
}


// add a linear combination: z <- z + sum_k alphas[k] * vectors[k] (VecMAXPY)
TEST(Math, PETScVectorAddLinearCombination)
{
    // x = (1, 2, 3, 4, 5)
    auto x = mito::linalg::petsc::vector("x", N);
    fill_vector(x, [](int i) { return double(i + 1); });

    // y = (1, 4, 9, 16, 25)
    auto y = mito::linalg::petsc::vector("y", N);
    fill_vector(y, [](int i) { return double((i + 1) * (i + 1)); });

    // z = (1, 1, 1, 1, 1)
    auto z = mito::linalg::petsc::vector("z", N);
    fill_vector(z, [](int) { return 1.0; });

    // z <- z + 2 * x - 1 * y
    const auto alphas = std::array<mito::linalg::petsc::scalar_t, 2>{ 2.0, -1.0 };
    const auto vectors = std::array<const mito::linalg::petsc::vector_t *, 2>{ &x, &y };
    z.add(alphas, vectors);

    // check the solution
    auto result = to_std_vector(z);
    EXPECT_DOUBLE_EQ(result[0], 2.0);
    EXPECT_DOUBLE_EQ(result[1], 1.0);
    EXPECT_DOUBLE_EQ(result[2], -2.0);
    EXPECT_DOUBLE_EQ(result[3], -7.0);
    EXPECT_DOUBLE_EQ(result[4], -14.0);

    // all done
    return;
}


TEST(Math, PETScVectorNorm)
{
    // x = (3, 4, 0, 0, 0), whose 2-norm is 5
    auto x = mito::linalg::petsc::vector("x", N);
    x.insert_value(0, 3.0);
    x.insert_value(1, 4.0);
    x.assemble();

    // check the returned norm (BLAS nrm2 rescales internally, so allow for round-off)
    EXPECT_NEAR(x.norm(), 5.0, 1e-14);

    // check the norm written to the output argument
    mito::linalg::petsc::scalar_t norm = -1.0;
    x.norm(norm);
    EXPECT_NEAR(norm, 5.0, 1e-14);

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