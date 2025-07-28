// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::matrix_solvers::petsc {

    // petsc linear system
    auto linear_system(const std::string & name)
    {
        return linear_system_t(name);
    }

    // petsc Krylov solver
    auto ksp(linear_system_t & linear_system)
    {
        return ksp_t(linear_system);
    }
}


// end of file
