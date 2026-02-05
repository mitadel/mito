// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // Factory: create induced metric from Jacobian field and parametrization
    template <
        int N, coordinates_c coordsT, coordinates_c parametric_coordsT, class JacobianT,
        class ParamT>
    constexpr auto induced_metric(JacobianT && jacobian, ParamT && parametrization)
    {
        return InducedMetric<
            N, coordsT, parametric_coordsT, std::decay_t<JacobianT>, std::decay_t<ParamT>>(
            std::forward<JacobianT>(jacobian), std::forward<ParamT>(parametrization));
    }
}    // namespace mito::geometry


// end of file
