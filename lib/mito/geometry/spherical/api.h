// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry::spherical {

    // the type of cartesian coordinates in {D} dimensions
    template <int D>
    using coordinates_t = geometry::coordinates_t<D, geometry::SPHERICAL>;

    // the metric tensor field
    constexpr auto metric = geometry::metric<spherical_coordinates_t>::field();

    // factory for spherical coordinates
    constexpr auto coordinates = &geometry::coordinates<spherical_coordinates_t>;

    // the function extracting the {r} coordinate from a spherical coordinate set
    constexpr auto r = functions::component<spherical_coordinates_t, 0>;

    // the function extracting the {theta} coordinate from a spherical coordinate set
    constexpr auto theta = functions::component<spherical_coordinates_t, 1>;

    // the function extracting the {phi} coordinate from a spherical coordinate set
    constexpr auto phi = functions::component<spherical_coordinates_t, 2>;

    // the basis for vector fields (e_r)
    constexpr auto e_r = basis<spherical_coordinates_t>::template e<0>();

    // the basis for vector fields (e_theta)
    constexpr auto e_theta = basis<spherical_coordinates_t>::template e<1>();

    // the basis for vector fields (e_phi)
    constexpr auto e_phi = basis<spherical_coordinates_t>::template e<2>();

}


// end of file
