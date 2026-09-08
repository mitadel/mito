// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::transient {

    // explicit Euler solver alias
    template <class discreteSystemT, class matrixSolverT, class linearSystemT>
    using explicit_euler_t = ExplicitEuler<discreteSystemT, matrixSolverT, linearSystemT>;

    // explicit Euler solver factory
    template <class discreteSystemT, class matrixSolverT, class linearSystemT>
    constexpr auto explicit_euler(discreteSystemT & discrete_system);
}


// end of file
