// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::math_backend::petsc {

    // petsc environment
    using environment_t = Environment;

    // petsc matrix
    using matrix_t = Matrix;

    // petsc vector
    using vector_t = Vector;

    // petsc backend
    using backend_t = Backend;

    // petsc linear system
    using linear_system_t = PETScLinearSystem;

    // petsc Krlov solver
    using ksp_t = PETScKrylovSolver;
}


// end of file
