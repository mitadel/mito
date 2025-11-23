// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, PETScInitializeFinalize)
{
    // initialize PETSc
    mito::petsc::initialize();

    // finalize PETSc
    mito::petsc::finalize();
}


// end of file
