// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // implicit euler factory
    template <linalg::valid_backend_c linalgBackendT, class semiDiscreteModelT, class solverT>
    constexpr auto implicit_euler(
        const semiDiscreteModelT & semi_discrete_model, solverT && solver,
        const fields::field_c auto & initial_condition, typename linalgBackendT::scalar_type dt)
        -> implicit_euler_t<semiDiscreteModelT, solverT, linalgBackendT>
    {
        return implicit_euler_t<semiDiscreteModelT, solverT, linalgBackendT>(
            semi_discrete_model, std::forward<solverT>(solver), initial_condition, dt);
    }

    // crank-nicholson factory
    template <linalg::valid_backend_c linalgBackendT, class semiDiscreteModelT, class solverT>
    constexpr auto crank_nicholson(
        const semiDiscreteModelT & semi_discrete_model, solverT && solver,
        const fields::field_c auto & initial_condition, typename linalgBackendT::scalar_type dt)
        -> crank_nicholson_t<semiDiscreteModelT, solverT, linalgBackendT>
    {
        return crank_nicholson_t<semiDiscreteModelT, solverT, linalgBackendT>(
            semi_discrete_model, std::forward<solverT>(solver), initial_condition, dt);
    }

}


// end of file
