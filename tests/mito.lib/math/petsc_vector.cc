// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Math, PETScVector)
{
    // the size of the vector
    int N = 10;

    // instantiate a PETSc vector of size {N}
    auto vector = mito::math_backend::petsc::vector("myvector");
    // create the vector and allocate the memory
    vector.create(N);

    // set vector entries
    for (int i = 0; i < N; i++) {
        vector.add_value(i, double(i * i));
        vector.add_value(i, double(i));
    }

    // assemble the vector
    vector.assemble();

    // set one entry to a specific value
    vector.insert_value(4, -50.0);

    // assemble the vector
    vector.assemble();

    // read the solution
    auto result = std::vector<double>(N);
    vector.get_vector(result);

    // check the solution
    EXPECT_DOUBLE_EQ(result[0], 0.0);
    EXPECT_DOUBLE_EQ(result[1], 2.0);
    EXPECT_DOUBLE_EQ(result[2], 6.0);
    EXPECT_DOUBLE_EQ(result[3], 12.0);
    EXPECT_DOUBLE_EQ(result[4], -50.0);
    EXPECT_DOUBLE_EQ(result[5], 30.0);
    EXPECT_DOUBLE_EQ(result[6], 42.0);
    EXPECT_DOUBLE_EQ(result[7], 56.0);
    EXPECT_DOUBLE_EQ(result[8], 72.0);
    EXPECT_DOUBLE_EQ(result[9], 90.0);

    // destroy the vector and free the memory
    vector.destroy();

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
