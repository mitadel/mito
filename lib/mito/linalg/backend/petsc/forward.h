// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once
#include <petscsystypes.h>


namespace mito::math_backend::petsc {

    // class for PETSc environment
    class Environment;
    // class for PETSc matrix
    class Matrix;
    // class for PETSc vector
    class Vector;
    // class for index type
    using index_type = PetscInt;
    // class for scalar type
    using scalar_type = PetscScalar;

    // struct for backend
    struct Backend {
        // the index type
        using index_type = PetscInt;
        // the scalar type
        using scalar_type = PetscScalar;
        // the matrix type
        using matrix_type = Matrix;
        // the vector type
        using vector_type = Vector;
    };

}


// end of file
