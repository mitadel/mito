// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, CUDADenseSolver)
{
    // the size of the linear system
    int N = 10;

    // instantiate a CUDA Dense solver for a linear system of size {N}
    auto solver = mito::solvers::cuda::dense(mito::solvers::cuda::SolverType::CHOLESKY);
    solver.initialize(N);

    // set matrix and right-hand side entries
    for (int i = 0; i < N; i++) {
        solver.set_matrix_value(i, i, 2.0, mito::solvers::cuda::InsertMode::INSERT_VALUE);
        if (i > 0) {
            solver.set_matrix_value(i, i - 1, -1.0, mito::solvers::cuda::InsertMode::INSERT_VALUE);
        }
        if (i < N - 1) {
            solver.set_matrix_value(i, i + 1, -1.0, mito::solvers::cuda::InsertMode::INSERT_VALUE);
        }
        solver.set_rhs_value(i, 1.0, mito::solvers::cuda::InsertMode::INSERT_VALUE);
    }

    // finalize the assembly of the linear system
    solver.finalize_assembly();

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


int
main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    // all done
    return result;
}


// end of file
