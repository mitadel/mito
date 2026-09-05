// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::transient {

    // explicit Euler solver alias
    template <class discreteSystemT, class matrixSolverT>
    using explicit_euler_t = ExplicitEuler<discreteSystemT, matrixSolverT>;

    // explicit Euler solver factory
    template <class discreteSystemT, class matrixSolverT>
    constexpr auto explicit_euler(discreteSystemT & discrete_system);
}


// end of file
