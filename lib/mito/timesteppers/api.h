// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // implicit euler alias
    template <class semiDiscreteModelT, class solverT, linalg::valid_backend_c linalgBackendT>
    using implicit_euler_t = ImplicitEuler<semiDiscreteModelT, solverT, linalgBackendT>;

    // implicit euler factory
    template <linalg::valid_backend_c linalgBackendT, class semiDiscreteModelT, class solverT>
    constexpr auto implicit_euler(
        const semiDiscreteModelT & semi_discrete_model, solverT && solver,
        const fields::field_c auto & initial_condition, typename linalgBackendT::scalar_type dt)
        -> implicit_euler_t<semiDiscreteModelT, solverT, linalgBackendT>;

    // crank-nicholson alias
    template <class semiDiscreteModelT, class solverT, linalg::valid_backend_c linalgBackendT>
    using crank_nicholson_t = CrankNicholson<semiDiscreteModelT, solverT, linalgBackendT>;

    // crank-nicholson factory
    template <linalg::valid_backend_c linalgBackendT, class semiDiscreteModelT, class solverT>
    constexpr auto crank_nicholson(
        const semiDiscreteModelT & semi_discrete_model, solverT && solver,
        const fields::field_c auto & initial_condition, typename linalgBackendT::scalar_type dt)
        -> crank_nicholson_t<semiDiscreteModelT, solverT, linalgBackendT>;

}


// end of file
