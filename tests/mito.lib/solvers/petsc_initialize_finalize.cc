// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, PETScInitializeFinalize)
{
    // initialize PETSc
    mito::solvers::petsc::initialize();

    // finalize PETSc
    mito::solvers::petsc::finalize();
}


// end of file
