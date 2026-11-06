// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    // implicit euler factory
    template <linalg::valid_backend_c linalgBackendT, class semiDiscreteModelT>
    constexpr auto implicit_euler(
        const semiDiscreteModelT & semi_discrete_model, fields::field_c auto initial_condition)
    {
        return implicit_euler_t<semiDiscreteModelT, linalgBackendT>(
            semi_discrete_model, initial_condition);
    }

}


// end of file
