// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry::cartesian {

    // the metric tensor field
    template <int D>
    constexpr auto metric = geometry::metric<cartesian_coordinates_t<D>>::field();

    // factory for cartesian coordinates
    template <int D>
    constexpr auto coordinates(mito::scalar_t (&&coord)[D])
    {
        return geometry::coordinates<cartesian_coordinates_t<D>>(std::move(coord));
    }

    // the function extracting the 0-th coordinate from a cartesian coordinate set
    template <int D>
    requires(D > 0)
    constexpr auto x_0 = functions::component<cartesian_coordinates_t<D>, 0>;

    // the function extracting the 1-st coordinate from a cartesian coordinate set
    template <int D>
    requires(D > 1)
    constexpr auto x_1 = functions::component<cartesian_coordinates_t<D>, 1>;

    // the function extracting the 2-nd coordinate from a cartesian coordinate set
    template <int D>
    requires(D > 2)
    constexpr auto x_2 = functions::component<cartesian_coordinates_t<D>, 1>;

    // the basis for vector fields (e_0)
    template <int D>
    requires(D > 0)
    constexpr auto e_0 = basis<cartesian_coordinates_t<D>>::template e<0>();

    // the basis for vector fields (e_1)
    template <int D>
    requires(D > 1)
    constexpr auto e_1 = basis<cartesian_coordinates_t<D>>::template e<1>();

    // the basis for vector fields (e_2)
    template <int D>
    requires(D > 2)
    constexpr auto e_2 = basis<cartesian_coordinates_t<D>>::template e<2>();

}


// end of file
