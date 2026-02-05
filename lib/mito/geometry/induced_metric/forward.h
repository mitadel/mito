// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // class induced metric (N-dimensional submanifold in ambient coordsT)
    template <
        int N, coordinates_c coordsT, coordinates_c parametric_coordsT, class JacobianFieldT,
        class ParametrizationT>
    class InducedMetric;

    // alias for induced metric type
    template <
        int N, coordinates_c coordsT, coordinates_c parametric_coordsT, class JacobianFieldT,
        class ParametrizationT>
    using induced_metric_t =
        InducedMetric<N, coordsT, parametric_coordsT, JacobianFieldT, ParametrizationT>;

}


// end of file
