// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // class discrete field
    template <class keyT, class Y>
    class DiscreteField;

    // nodal field
    template <int D, class Y>
    using nodal_field_t = DiscreteField<geometry::node_t<D>, Y>;

    // point field
    template <int D, class Y>
    using point_field_t = DiscreteField<geometry::point_t<D>, Y>;

    // class quadrature field
    template <int Q, class Y>
    class QuadratureField;

    // quadrature field alias
    template <int Q, class Y>
    using quadrature_field_t = QuadratureField<Q, Y>;
}


// end of file
