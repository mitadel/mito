// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>

TEST(Solvers, CUDASparseSolverSymmetricLinearSystem)
{
    // the size of the linear system
    int N = 10;

    // numerical tolerance for the solution
    double tol = 1.0e-10;

    // instantiate a CUDA Sparse solver for a linear system of size {N}
    auto solver =
        mito::solvers::cuda::sparse<mito::real>(mito::solvers::cuda::SolverType::CHOLESKY);
    solver.initialize(N, 3);    // 3 non-zero entries per row

    // set matrix and right-hand side entries
    for (int i = 0; i < N; i++) {
        // matrix: diagonal
        solver.set_matrix_value(i, i, 2.0, mito::solvers::cuda::InsertMode::INSERT_VALUE);
        // matrix: subdiagonal
        if (i > 0) {
            solver.set_matrix_value(i, i - 1, -1.0, mito::solvers::cuda::InsertMode::ADD_VALUE);
        }
        // matrix: superdiagonal
        if (i < N - 1) {
            solver.set_matrix_value(i, i + 1, -1.0, mito::solvers::cuda::InsertMode::INSERT_VALUE);
        }
        // rhs
        solver.set_rhs_value(i, 1.0, mito::solvers::cuda::InsertMode::ADD_VALUE);
    }

    // finalize the assembly of the linear system
    solver.finalize_assembly();

    // solve the linear system
    solver.solve();

    // read the solution
    auto x = std::vector<double>(N);
    solver.get_solution(x);

    // check the solution
    EXPECT_NEAR(x[0], 5.0, tol);
    EXPECT_NEAR(x[1], 9.0, tol);
    EXPECT_NEAR(x[2], 12.0, tol);
    EXPECT_NEAR(x[3], 14.0, tol);
    EXPECT_NEAR(x[4], 15.0, tol);
    EXPECT_NEAR(x[5], 15.0, tol);
    EXPECT_NEAR(x[6], 14.0, tol);
    EXPECT_NEAR(x[7], 12.0, tol);
    EXPECT_NEAR(x[8], 9.0, tol);
    EXPECT_NEAR(x[9], 5.0, tol);

    // all done
    return;
}

TEST(Solvers, CUDASparseSolverUnsymmetricLinearSystem)
{
    // the size of the linear system
    int N = 10;

    // numerical tolerance for the solution
    double tol = 1.0e-10;

    // instantiate a CUDA Sparse solver for a linear system of size {N}
    auto solver = mito::solvers::cuda::sparse<mito::real>(mito::solvers::cuda::SolverType::LU);
    solver.initialize(N);    // non-zeros per row are not set

    // set matrix and right-hand side entries
    for (int i = 0; i < N; i++) {
        // matrix: diagonal
        solver.set_matrix_value(i, i, 2.0, mito::solvers::cuda::InsertMode::ADD_VALUE);
        // matrix: subdiagonal
        if (i > 0) {
            solver.set_matrix_value(i, i - 1, -1.0, mito::solvers::cuda::InsertMode::INSERT_VALUE);
        }
        // matrix: superdiagonal
        if (i < N - 1) {
            solver.set_matrix_value(i, i + 1, -0.5, mito::solvers::cuda::InsertMode::INSERT_VALUE);
        }
        // rhs
        solver.set_rhs_value(i, 1.0, mito::solvers::cuda::InsertMode::ADD_VALUE);
    }

    // finalize the assembly of the linear system
    solver.finalize_assembly();

    // solve the linear system
    solver.solve();

    // read the solution
    auto x = std::vector<double>(N);
    solver.get_solution(x);

    // check the solution
    EXPECT_NEAR(x[0], 0.8284194482079074, tol);
    EXPECT_NEAR(x[1], 1.3136777928316294, tol);
    EXPECT_NEAR(x[2], 1.5978722749107030, tol);
    EXPECT_NEAR(x[3], 1.7641335139795535, tol);
    EXPECT_NEAR(x[4], 1.8607895060968098, tol);
    EXPECT_NEAR(x[5], 1.9148909964281315, tol);
    EXPECT_NEAR(x[6], 1.9379849735189063, tol);
    EXPECT_NEAR(x[7], 1.9221579012193621, tol);
    EXPECT_NEAR(x[8], 1.8126616578396353, tol);
    EXPECT_NEAR(x[9], 1.4063308289198175, tol);

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
