// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    // package backend types in a struct
    struct Backend {
        // the index type
        using index_type = index_t;
        // the scalar type
        using scalar_type = scalar_t;
        // the vector type
        using vector_type = vector_t;
        // the matrix type
        using matrix_type = matrix_t;
    };

}    // namespace mito::linalg::petsc


// end of file
