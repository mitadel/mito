// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    // environment
    using environment_t = Environment;

    // backend
    using backend_t = Backend;

    // int
    using int_t = backend_t::int_type;

    // scalar
    using scalar_t = backend_t::scalar_type;

    // vector
    using vector_t = backend_t::vector_type;

    // matrix
    using matrix_t = backend_t::matrix_type;

    // linear system
    using linear_system_t = LinearSystem;

    // Krlov solver
    using ksp_t = KrylovSolver;
}


// end of file
