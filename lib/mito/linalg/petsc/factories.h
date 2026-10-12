// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    // environment
    inline auto environment() -> environment_t
    {
        return environment_t();
    }

    // matrix
    inline auto matrix(const std::string & name, index_t size)
    {
        return matrix_t(name, size);
    }

    // vector
    inline auto vector(const std::string & name, index_t size)
    {
        return vector_t(name, size);
    }

    // linear system
    inline auto linear_system(const std::string & name, index_t size)
    {
        return linear_system_t(name, size);
    }

    // Krylov solver
    inline auto ksp(linear_system_t & linear_system)
    {
        return ksp_t(linear_system);
    }
}


// end of file
