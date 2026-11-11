// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // linear solver alias
    template <class semiDiscreteModelT, class solverT, linalg::valid_backend_c linalgBackendT>
    using implicit_euler_t = ImplicitEuler<semiDiscreteModelT, solverT, linalgBackendT>;

    // linear solver factory
    template <linalg::valid_backend_c linalgBackendT, class semiDiscreteModelT, class solverT>
    constexpr auto implicit_euler(
        const semiDiscreteModelT & semi_discrete_model, solverT && solver,
        const fields::field_c auto & initial_condition, typename linalgBackendT::scalar_type dt)
        -> implicit_euler_t<semiDiscreteModelT, solverT, linalgBackendT>;
}


// end of file
