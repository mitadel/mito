// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::math_backend::petsc {

    // petsc environment
    auto environment() -> environment_t
    {
        return environment_t();
    }

    // petsc matrix
    auto matrix(const std::string & name)
    {
        return matrix_t(name);
    }

    // petsc vector
    auto vector(const std::string & name)
    {
        return vector_t(name);
    }

}


// end of file
