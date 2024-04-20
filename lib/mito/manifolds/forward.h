// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <class cellT, geometry::CoordinateType coordsT>
    class Manifold;

    // class P-form
    template <int P, class F>
    class Form;

    // class field
    template <geometry::CoordinateType coordT, functions::function_c F>
    requires(vector_c<typename F::input_type>)
    class Field;
}


#endif

// end of file
