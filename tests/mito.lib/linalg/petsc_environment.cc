// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito.h>


TEST(Solvers, PETScEnvironment)
{
    // create petsc environment
    auto environment = mito::linalg::backend::petsc::environment();
}


// end of file
