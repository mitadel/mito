// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fem {

    // class nodal field
    template <class Y>
    class NodalField;

    // nodal field alias
    template <class Y>
    using nodal_field_t = NodalField<Y>;

    // class quadrature field
    template <int Q, class Y>
    class QuadratureField;

    // quadrature field alias
    template <int Q, class Y>
    using quadrature_field_t = QuadratureField<Q, Y>;
}


// end of file
