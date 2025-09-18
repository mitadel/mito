// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, CUDADenseSolverSymmetricLinearSystem)
{
    // the size of the linear system
    int N = 10;

    // numerical tolerance for the solution
    double tol = 1.0e-10;

    // instantiate a CUDA Dense solver for a linear system of size {N}
    auto solver = mito::solvers::cuda::dense<mito::real>(mito::solvers::cuda::SolverType::CHOLESKY);
    solver.initialize(N);

    // set matrix entries
    double A[10][10] = { { 3.5, -1.2, 0.8, 0.0, -0.5, 1.1, 0.0, 0.3, -0.9, 0.4 },
                         { -1.2, 4.0, 1.3, -0.4, 0.6, -0.7, 0.0, 0.5, 0.0, -1.0 },
                         { 0.8, 1.3, 3.8, 0.7, -0.8, 0.0, -0.2, 0.6, -0.5, 0.0 },
                         { 0.0, -0.4, 0.7, 4.2, 1.0, 0.9, 0.3, 0.0, 0.4, -0.3 },
                         { -0.5, 0.6, -0.8, 1.0, 4.5, 0.4, -0.7, 0.8, 0.0, 0.2 },
                         { 1.1, -0.7, 0.0, 0.9, 0.4, 3.6, -0.3, 0.0, -0.4, 0.5 },
                         { 0.0, 0.0, -0.2, 0.3, -0.7, -0.3, 3.3, 0.6, -0.8, 0.0 },
                         { 0.3, 0.5, 0.6, 0.0, 0.8, 0.0, 0.6, 4.1, 0.2, 0.7 },
                         { -0.9, 0.0, -0.5, 0.4, 0.0, -0.4, -0.8, 0.2, 3.7, 0.9 },
                         { 0.4, -1.0, 0.0, -0.3, 0.2, 0.5, 0.0, 0.7, 0.9, 4.0 } };
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            solver.set_matrix_value(i, j, A[i][j], mito::solvers::cuda::InsertMode::INSERT_VALUE);
        }
    }

    // set rhs entries
    double b[10] = { 1.0, -2.0, 0.5, 1.5, -1.0, 0.7, -0.8, 2.0, 0.0, -1.5 };
    for (int i = 0; i < N; i++) {
        solver.set_rhs_value(i, b[i], mito::solvers::cuda::InsertMode::INSERT_VALUE);
    }

    // finalize the assembly of the linear system
    solver.finalize_assembly();

    // solve the linear system
    solver.solve();

    // read the solution
    auto x = std::vector<double>(N);
    solver.get_solution(x);

    // check the solution
    EXPECT_NEAR(x[0], -0.0560503759, tol);
    EXPECT_NEAR(x[1], -0.6932480517, tol);
    EXPECT_NEAR(x[2], 0.0666007599, tol);
    EXPECT_NEAR(x[3], 0.3544482567, tol);
    EXPECT_NEAR(x[4], -0.4084514057, tol);
    EXPECT_NEAR(x[5], 0.0767339355, tol);
    EXPECT_NEAR(x[6], -0.5097195863, tol);
    EXPECT_NEAR(x[7], 0.8324810769, tol);
    EXPECT_NEAR(x[8], -0.0333448254, tol);
    EXPECT_NEAR(x[9], -0.6434741305, tol);

    // all done
    return;
}

TEST(Solvers, CUDADenseSolverUnsymmetricLinearSystem)
{
    // the size of the linear system
    int N = 10;

    // numerical tolerance for the solution
    double tol = 1.0e-10;

    // instantiate a CUDA Dense solver for a linear system of size {N}
    auto solver = mito::solvers::cuda::dense<mito::real>(mito::solvers::cuda::SolverType::LU);
    solver.initialize(N);

    // set matrix entries
    double A[10][10] = { { 3.5, -1.2, 0.8, 0.0, -0.5, 1.1, 0.0, 0.3, -0.9, 0.4 },
                         { 0.6, 4.0, 1.3, -0.4, 0.6, -0.7, 0.0, 0.5, 0.0, -1.0 },
                         { -1.1, 1.0, 3.8, 0.7, -0.8, 0.0, -0.2, 0.6, -0.5, 0.0 },
                         { 0.0, -0.4, 0.7, 4.2, -0.9, 0.9, 0.3, 0.0, 0.4, -0.3 },
                         { 0.9, 0.0, -0.8, 1.0, 4.5, 0.4, -0.7, 0.8, 0.0, 0.2 },
                         { 1.1, -0.7, 0.0, -1.3, 0.4, 3.6, -0.3, 0.0, -0.4, 0.5 },
                         { 0.0, 0.0, 1.1, 0.3, -0.7, -0.3, 3.3, 0.6, -0.8, 0.0 },
                         { 0.3, 0.5, 0.6, 0.0, 0.8, 0.0, 0.6, 4.1, -0.5, 0.7 },
                         { -0.9, 0.0, -0.5, -1.0, 0.0, -0.4, -0.8, 0.2, 3.7, 0.9 },
                         { 0.4, 1.3, 0.0, -0.3, 0.2, 0.5, 0.0, 0.7, 0.9, 4.0 } };
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            solver.set_matrix_value(i, j, A[i][j], mito::solvers::cuda::InsertMode::INSERT_VALUE);
        }
    }

    // set rhs entries
    double b[10] = { -1.2, 0.5, 0.7, 1.0, 0.3, -1.5, 2.0, -0.8, 0.6, 1.1 };
    for (int i = 0; i < N; i++) {
        solver.set_rhs_value(i, b[i], mito::solvers::cuda::InsertMode::INSERT_VALUE);
    }

    // finalize the assembly of the linear system
    solver.finalize_assembly();

    // solve the linear system
    solver.solve();

    // read the solution
    auto x = std::vector<double>(N);
    solver.get_solution(x);

    // check the solution
    EXPECT_NEAR(x[0], -0.1642774470, tol);
    EXPECT_NEAR(x[1], 0.1352982046, tol);
    EXPECT_NEAR(x[2], 0.2493049978, tol);
    EXPECT_NEAR(x[3], 0.2587528172, tol);
    EXPECT_NEAR(x[4], 0.2711567201, tol);
    EXPECT_NEAR(x[5], -0.2258981243, tol);
    EXPECT_NEAR(x[6], 0.6823460181, tol);
    EXPECT_NEAR(x[7], -0.4005242029, tol);
    EXPECT_NEAR(x[8], 0.3015605175, tol);
    EXPECT_NEAR(x[9], 0.2837823381, tol);

    // all done
    return;
}


int
main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    // all done
    return result;
}


// end of file
