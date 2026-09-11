// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::matrix_solvers::petsc {

    // petsc linear system
    auto linear_system(const std::string & name)
    {
        return linear_system_t(name);
    }

    // Krylov solver
    auto ksp(linear_system_t & linear_system)
    {
        return ksp_t(linear_system);
    }

    // petsc Krylov solver
    auto petsc_ksp(linear_system_t & linear_system)
    {
        return petsc_ksp_t(linear_system);
    }
}


// end of file
