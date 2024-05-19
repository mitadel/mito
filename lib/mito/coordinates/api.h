// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_coordinates_api_h)
#define mito_coordinates_api_h


namespace mito::geometry {

    // coordinates alias in D-dimensions
    template <int D, CoordinateType coordT>
    using coordinates_t = Coordinates<D, coordT>;
}


#endif

// end of file
