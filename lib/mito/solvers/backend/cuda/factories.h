// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    // cuda dense solver
    auto dense(mito::solvers::cuda::SolverType solver_type = mito::solvers::cuda::SolverType::LU)
    {
        return dense_t(solver_type);
    }

}


// end of file