// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::math_backend::petsc {

    // class for PETSc matrix
    class Matrix;
    // class for PETSc vector
    class Vector;

    // struct for backend
    struct PETScBackend {
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
