// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::matrix_solvers::petsc {

    // petsc linear system
    using linear_system_t = PETScLinearSystem;

    // petsc Krlov solver
    using ksp_t = PETScKrylovSolver;

}


// end of file
