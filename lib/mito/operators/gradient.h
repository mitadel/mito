// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::operators {

    // full-manifold gradient: grad f = g^{-1} * (df/dx)
    constexpr auto gradient(
        const fields::scalar_field_c auto & f,
        const auto & metric_field)
    {
        auto g_inv = functions::inverse(metric_field);
        auto df = fields::gradient(f);
        return g_inv * df;
    }

    // submanifold gradient: grad f = J * (g_induced^{-1} * df/dxi), result in ambient space
    constexpr auto gradient(
        const fields::scalar_field_c auto & f,
        const auto & metric_field,
        const auto & jacobian_field)
    {
        auto g_inv = functions::inverse(metric_field);
        auto df = fields::gradient(f);
        return jacobian_field * (g_inv * df);
    }

}


// end of file
