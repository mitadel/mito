// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, PETScKSPSolver)
{
    // the size of the linear system
    int N = 10;

    // instantiate a PETSc Krylov solver for a linear system of size {N}
    mito::solvers::petsc::PETScKrylovSolver solver;
    solver.initialize(N);
    solver.set_options({});

    // set matrix and right-hand side entries
    for (int i = 0; i < N; i++) {
        solver.insert_matrix_value(i, i, 2.0);
        if (i > 0) {
            solver.insert_matrix_value(i, i - 1, -1.0);
        }
        if (i < N - 1) {
            solver.insert_matrix_value(i, i + 1, -1.0);
        }
        solver.insert_rhs_value(i, 1.0);
    }

    // solve the linear system
    solver.solve();

    // read the solution
    auto x = std::vector<double>(N);
    solver.get_solution(x);

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

    // finalize the solver
    solver.finalize();

    // all done
    return;
}


// end of file
