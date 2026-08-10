// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry::cartesian {

    // the type of cartesian coordinates in {D} dimensions
    template <int D>
    using coordinates_t = cartesian_coordinates_t<D>;

    // the Euclidean metric tensor field
    template <int D>
    constexpr auto euclidean_metric =
        geometry::euclidean_metric<cartesian_coordinates_t<D>>::field();

    // factory for cartesian coordinates
    template <int D>
    constexpr auto coordinates(mito::tensor::scalar_t (&&coord)[D])
    {
        return geometry::coordinates<cartesian_coordinates_t<D>>(std::move(coord));
    }

    // the function extracting the 0-th coordinate from a cartesian coordinate set
    template <int D>
    requires(D > 0)
    constexpr auto x = functions::component<cartesian_coordinates_t<D>, 0>;

    // the function extracting the 1-st coordinate from a cartesian coordinate set
    template <int D>
    requires(D > 1)
    constexpr auto y = functions::component<cartesian_coordinates_t<D>, 1>;

    // the function extracting the 2-nd coordinate from a cartesian coordinate set
    template <int D>
    requires(D > 2)
    constexpr auto z = functions::component<cartesian_coordinates_t<D>, 2>;

    // the basis for vector fields (e_0)
    template <int D>
    requires(D > 0)
    constexpr auto e_x = basis<cartesian_coordinates_t<D>>::template e<0>();

    // the basis for vector fields (e_1)
    template <int D>
    requires(D > 1)
    constexpr auto e_y = basis<cartesian_coordinates_t<D>>::template e<1>();

    // the basis for vector fields (e_2)
    template <int D>
    requires(D > 2)
    constexpr auto e_z = basis<cartesian_coordinates_t<D>>::template e<2>();

}


// end of file
