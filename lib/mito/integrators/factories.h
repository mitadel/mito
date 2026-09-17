// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::integrators {


    // explicit Euler factory
    template <class matrixSolverT, class mathBackendT, class discreteSystemT>
    constexpr auto explicit_euler(discreteSystemT & discrete_system)
    {
        return explicit_euler_t<discreteSystemT, matrixSolverT, mathBackendT>(discrete_system);
    }

}


// end of file
