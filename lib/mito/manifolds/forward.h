// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <geometry::CoordinateType coordsT, class cellT, int D>
    class Manifold;

    // class P-form
    template <int P, class F>
    class Form;

    // class field
    template <class F>
    class Field;
}


#endif

// end of file
