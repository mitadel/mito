// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_cartesian_api_h)
#define mito_geometry_cartesian_api_h


namespace mito::geometry::cartesian {

    // the metric tensor field
    template <int D>
    constexpr auto metric = geometry::metric<cartesian_coordinates_t<D>>::field();

    // factory for cartesian coordinates
    template <int D>
    constexpr auto coordinates = &geometry::coordinates<cartesian_coordinates_t<D>>;

    // the basis for vector fields (e_0)
    template <int D>
    constexpr auto e_0 = basis<cartesian_coordinates_t<D>>::template e<0>();

    // the basis for vector fields (e_1)
    template <int D>
    constexpr auto e_1 = basis<cartesian_coordinates_t<D>>::template e<1>();

    // the basis for vector fields (e_2)
    template <int D>
    constexpr auto e_2 = basis<cartesian_coordinates_t<D>>::template e<2>();

}


#endif

// end of file
