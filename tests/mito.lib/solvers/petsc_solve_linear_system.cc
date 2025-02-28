// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


int
main()
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

    // finalize the solver
    solver.finalize();

    // all done
    return 0;
}