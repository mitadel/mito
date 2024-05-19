// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_cartesian_forward_h)
#define mito_geometry_cartesian_forward_h


namespace mito::geometry {

    // cartesian coordinates type
    template <int D>
    using cartesian_coordinates_t = coordinates_t<D, CARTESIAN>;

}


#endif

// end of file
