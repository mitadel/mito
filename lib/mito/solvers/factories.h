// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers {

    // linear factory
    template <linalg::valid_backend_c linalgBackendT, class matrixSolverT, class discreteSystemT>
    constexpr auto linear_solver(discreteSystemT & discrete_system)
    {
        return linear_solver_t<discreteSystemT, matrixSolverT, linalgBackendT>(discrete_system);
    }

}


// end of file
