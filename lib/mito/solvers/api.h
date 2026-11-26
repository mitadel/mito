// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers {

    // linear solver alias
    template <class discreteSystemT, class matrixSolverT, linalg::valid_backend_c linalgBackendT>
    using linear_solver_t = LinearSolver<discreteSystemT, matrixSolverT, linalgBackendT>;

    // linear solver factory
    template <class discreteSystemT, class matrixSolverT, linalg::valid_backend_c linalgBackendT>
    constexpr auto linear_solver(discreteSystemT & discrete_system);
}


// end of file
