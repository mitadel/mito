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

    // instantiate a PETSc linear system of size {N}
    auto linear_system = mito::matrix_solvers::petsc::linear_system("mysystem");
    linear_system.create(N);

    // instantiate a PETSc Krylov solver for the linear system
    auto solver = mito::matrix_solvers::petsc::ksp(linear_system);
    solver.create();
    solver.set_options("-ksp_monitor");

    // set matrix and right-hand side entries
    for (int i = 0; i < N; i++) {
        linear_system.insert_matrix_value(i, i, 2.0);
        if (i > 0) {
            linear_system.insert_matrix_value(i, i - 1, -1.0);
        }
        if (i < N - 1) {
            linear_system.insert_matrix_value(i, i + 1, -1.0);
        }
        linear_system.insert_rhs_value(i, 1.0);
    }

    // solve the linear system
    solver.solve();

    // read the solution
    auto x = std::vector<double>(N);
    linear_system.get_solution(x);

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

    // destroy the system
    linear_system.destroy();

    // destroy the solver
    solver.destroy();

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
