// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // cartesian geometry in 1D
    template <int D>
    struct cartesian;

    // cartesian geometry in 1D Euclidean space
    template <>
    struct cartesian<1> {

        // the type of cartesian coordinates in 1D
        using coordinates_t = cartesian_coordinates_t<1>;

        // the Euclidean metric tensor field
        static constexpr auto euclidean_metric =
            geometry::euclidean_metric<cartesian_coordinates_t<1>>::field();

        // factory for cartesian coordinates
        static constexpr auto coordinates(mito::tensor::scalar_t (&&coord)[1])
        {
            return geometry::coordinates<cartesian_coordinates_t<1>>(std::move(coord));
        }

        // the function extracting the 0-th coordinate from a cartesian coordinate set
        static constexpr auto x = functions::component<cartesian_coordinates_t<1>, 0>;

        // the basis for vector fields (e_0)
        static constexpr auto e_x = basis<cartesian_coordinates_t<1>>::template e<0>();
    };

    // cartesian geometry in 2D Euclidean space
    template <>
    struct cartesian<2> {

        // the type of cartesian coordinates in 2D
        using coordinates_t = cartesian_coordinates_t<2>;

        // the Euclidean metric tensor field
        static constexpr auto euclidean_metric =
            geometry::euclidean_metric<cartesian_coordinates_t<2>>::field();

        // factory for cartesian coordinates
        static constexpr auto coordinates(mito::tensor::scalar_t (&&coord)[2])
        {
            return geometry::coordinates<cartesian_coordinates_t<2>>(std::move(coord));
        }

        // the function extracting the 0-th coordinate from a cartesian coordinate set
        static constexpr auto x = functions::component<cartesian_coordinates_t<2>, 0>;

        // the function extracting the 1-st coordinate from a cartesian coordinate set
        static constexpr auto y = functions::component<cartesian_coordinates_t<2>, 1>;

        // the basis for vector fields (e_0)
        static constexpr auto e_x = basis<cartesian_coordinates_t<2>>::template e<0>();

        // the basis for vector fields (e_1)
        static constexpr auto e_y = basis<cartesian_coordinates_t<2>>::template e<1>();
    };

    // cartesian geometry in 3D Euclidean space
    template <>
    struct cartesian<3> {

        // the type of cartesian coordinates in 3D
        using coordinates_t = cartesian_coordinates_t<3>;

        // the Euclidean metric tensor field
        static constexpr auto euclidean_metric =
            geometry::euclidean_metric<cartesian_coordinates_t<3>>::field();

        // factory for cartesian coordinates
        static constexpr auto coordinates(mito::tensor::scalar_t (&&coord)[3])
        {
            return geometry::coordinates<cartesian_coordinates_t<3>>(std::move(coord));
        }

        // the function extracting the 0-th coordinate from a cartesian coordinate set
        static constexpr auto x = functions::component<cartesian_coordinates_t<3>, 0>;

        // the function extracting the 1-st coordinate from a cartesian coordinate set
        static constexpr auto y = functions::component<cartesian_coordinates_t<3>, 1>;

        // the function extracting the 2-nd coordinate from a cartesian coordinate set
        static constexpr auto z = functions::component<cartesian_coordinates_t<3>, 2>;

        // the basis for vector fields (e_0)
        static constexpr auto e_x = basis<cartesian_coordinates_t<3>>::template e<0>();

        // the basis for vector fields (e_1)
        static constexpr auto e_y = basis<cartesian_coordinates_t<3>>::template e<1>();

        // the basis for vector fields (e_2)
        static constexpr auto e_z = basis<cartesian_coordinates_t<3>>::template e<2>();
    };
}


// end of file
