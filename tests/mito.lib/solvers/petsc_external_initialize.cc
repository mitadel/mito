// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, PETScKSPExternalInitialize)
{
    // initialize PETSc
    PetscInitializeNoArguments();

    // the size of the linear system
    int N = 10;

    // instantiate a PETSc Krylov solver for a linear system of size {N}
    auto solver = mito::solvers::petsc::PETScKrylovSolver("mysolver");
    solver.initialize(N);

    // finalize the solver
    solver.finalize();

    // finalize PETSc
    PetscFinalize();
}


// end of file
