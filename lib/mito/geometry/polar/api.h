// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_polar_api_h)
#define mito_geometry_polar_api_h


namespace mito::geometry::polar {

    // the metric tensor field
    constexpr auto metric = geometry::metric<polar_coordinates_t>::field();

    // factory for polar coordinates
    constexpr auto coordinates = &geometry::coordinates<polar_coordinates_t>;

    // the basis for vector fields (e_r)
    constexpr auto e_r = basis<polar_coordinates_t>::template e<0>();

    // the basis for vector fields (e_theta)
    constexpr auto e_theta = basis<polar_coordinates_t>::template e<1>();

}


#endif

// end of file
