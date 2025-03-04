// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::petsc {

    // petsc ksp solver
    auto ksp(const std::string & name)
    {
        return ksp_t(name);
    }

}


// end of file
