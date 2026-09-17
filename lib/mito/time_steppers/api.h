// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::time_steppers {

    // explicit Euler solver alias
    template <class discreteSystemT, class matrixSolverT, class mathBackendT>
    using explicit_euler_t = ExplicitEuler<discreteSystemT, matrixSolverT, mathBackendT>;

    // explicit Euler solver factory
    template <class discreteSystemT, class matrixSolverT, class mathBackendT>
    constexpr auto explicit_euler(discreteSystemT & discrete_system);
}


// end of file
