// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // linear solver alias
    template <class semiDiscreteModelT, linalg::valid_backend_c linalgBackendT>
    using implicit_euler_t = ImplicitEuler<semiDiscreteModelT, linalgBackendT>;

    // linear solver factory
    template <linalg::valid_backend_c linalgBackendT, class semiDiscreteModelT>
    constexpr auto implicit_euler(const semiDiscreteModelT & semi_discrete_model);
}


// end of file
