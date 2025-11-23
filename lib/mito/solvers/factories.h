// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers {

    // linear factory
    template <class matrixSolverT, class discreteSystemT>
    constexpr auto linear_solver(discreteSystemT & discrete_system)
    {
        return linear_solver_t<discreteSystemT, matrixSolverT>(discrete_system);
    }

}


// end of file
