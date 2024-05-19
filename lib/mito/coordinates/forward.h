// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_coordinates_forward_h)
#define mito_coordinates_forward_h


namespace mito::geometry {

    // available coordinate types
    enum CoordinateType { CARTESIAN, POLAR, SPHERICAL };

    // class coordinates
    template <int D, CoordinateType coordT>
    class Coordinates;

    // concept of a set of coordinates
    template <class F>
    concept coordinates_c = requires(F c) {
        // require that F only binds to {Coordinates} specializations
        []<int D, CoordinateType coordT>(const Coordinates<D, coordT> &) {
        }(c);
    };
}


#endif

// end of file
