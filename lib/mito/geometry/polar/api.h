// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    struct polar {

        // polar coordinates
        using coordinates_t = polar_coordinates_t;

        // the Euclidean metric tensor field
        static constexpr auto euclidean_metric =
            geometry::euclidean_metric<polar_coordinates_t>::field();

        // factory for polar coordinates
        static constexpr auto coordinates = &geometry::coordinates<polar_coordinates_t>;

        // the function extracting the {r} coordinate from a polar coordinate set
        static constexpr auto r = functions::component<polar_coordinates_t, 0>;

        // the function extracting the {theta} coordinate from a polar coordinate set
        static constexpr auto theta = functions::component<polar_coordinates_t, 1>;

        // the basis for vector fields (e_r)
        static constexpr auto e_r = basis<polar_coordinates_t>::template e<0>();

        // the basis for vector fields (e_theta)
        static constexpr auto e_theta = basis<polar_coordinates_t>::template e<1>();
    };

}


// end of file
