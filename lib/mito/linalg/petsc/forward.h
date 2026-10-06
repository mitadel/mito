// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once
#include <petscsystypes.h>


namespace mito::linalg::petsc {

    // class for environment
    class Environment;

    // class for vector
    class Vector;

    // class for matrix
    class Matrix;

    // class for linear system
    class LinearSystem;

    // class for Krylov solver
    class KrylovSolver;

    // struct for backend
    struct Backend {
        // the index type
        using index_type = PetscInt;
        // the scalar type
        using scalar_type = PetscScalar;
        // the vector type
        using vector_type = Vector;
        // the matrix type
        using matrix_type = Matrix;
    };
}


// end of file
