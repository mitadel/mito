// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::cuda {

    // cuda dense solver
    template <real_c realT>
    auto dense(SolverType solver_type = SolverType::LU)
    {
        return dense_t<realT>(solver_type);
    }

    // cuda sparse solver
    template <real_c realT>
    auto sparse(SolverType solver_type = SolverType::LU)
    {
        return sparse_t<realT>(solver_type);
    }
}


// end of file