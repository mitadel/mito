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
    inline auto matrix(const std::string & name, index_t size, assembly_format format = assembly_format::csr)
    {
        return matrix_t(name, size, format);
    }

    // matrix copy
    inline auto matrix(const std::string & name, const matrix_t & other)
    {
        return matrix_t(name, other);
    }

    // vector
    inline auto vector(const std::string & name, index_t size)
    {
        return vector_t(name, size);
    }

    // Krylov solver
    inline auto ksp(linear_system_t & linear_system)
    {
        return ksp_t(linear_system);
    }
}


// end of file
