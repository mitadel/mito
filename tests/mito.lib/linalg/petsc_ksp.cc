// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, PETScKSPSolver)
{
    // the size of the linear system
    int N = 10;
    // linear algebra backend
    using backend_t = mito::linalg::petsc::backend_t;

    // instantiate a PETSc matrix of size {N} and a PETSc vector of size {N}
    auto LHS = backend_t::matrix_type("LHS", N);
    auto RHS = backend_t::vector_type("RHS", N);
    auto solution = backend_t::vector_type("solution", N);

    // instantiate a PETSc Krylov solver for the linear system
    auto solver = mito::linalg::petsc::ksp("mysystem");
    // set options for the petsc Krylov solver
    solver.set_options("-ksp_monitor");

    // set matrix and right-hand side entries
    for (int i = 0; i < N; i++) {
        LHS.insert_value(i, i, 2.0);
        if (i > 0) {
            LHS.insert_value(i, i - 1, -1.0);
        }
        if (i < N - 1) {
            LHS.insert_value(i, i + 1, -1.0);
        }
        RHS.insert_value(i, 1.0);
    }

    // solve the linear system
    solver.solve(LHS, RHS, solution);

    // read the solution
    auto x = std::vector<double>(N);
    solution.get_vector(x);

    // check the solution
    EXPECT_DOUBLE_EQ(x[0], 5.0);
    EXPECT_DOUBLE_EQ(x[1], 9.0);
    EXPECT_DOUBLE_EQ(x[2], 12.0);
    EXPECT_DOUBLE_EQ(x[3], 14.0);
    EXPECT_DOUBLE_EQ(x[4], 15.0);
    EXPECT_DOUBLE_EQ(x[5], 15.0);
    EXPECT_DOUBLE_EQ(x[6], 14.0);
    EXPECT_DOUBLE_EQ(x[7], 12.0);
    EXPECT_DOUBLE_EQ(x[8], 9.0);
    EXPECT_DOUBLE_EQ(x[9], 5.0);

    // all done
    return;
}


int
main(int argc, char ** argv)
{
    // create petsc environment
    auto environment = mito::linalg::petsc::environment();

    // initialize Google Test
    ::testing::InitGoogleTest(&argc, argv);

    // run all tests
    auto result = RUN_ALL_TESTS();

    // all done
    return result;
}


// end of file
