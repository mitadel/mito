// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    // environment
    using environment_t = Environment;

    // index
    using index_t = PetscInt;

    // scalar
    using scalar_t = PetscScalar;

    // vector
    using vector_t = utilities::NamedClass<Vector>;

    // matrix
    using matrix_t = utilities::NamedClass<Matrix>;

    // linear system
    using linear_system_t = utilities::NamedClass<LinearSystem>;

    // Krlov solver
    using ksp_t = KrylovSolver;

    // backend
    using backend_t = Backend;
}


// end of file
