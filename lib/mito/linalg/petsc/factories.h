// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    // environment
    auto environment() -> environment_t
    {
        return environment_t();
    }

    // matrix
    auto matrix(const std::string & name, index_type size)
    {
        return matrix_t(name, size);
    }

    // vector
    auto vector(const std::string & name, index_type size)
    {
        return vector_t(name, size);
    }

    // linear system
    auto linear_system(const std::string & name, index_type size)
    {
        return linear_system_t(name, size);
    }

    // Krylov solver
    auto ksp(linear_system_t & linear_system)
    {
        return ksp_t(linear_system);
    }
}


// end of file
