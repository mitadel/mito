// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fields.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Fields, VectorFields)
{
    // the cosine function
    constexpr auto cos = mito::functions::cos;
    // the function extracting the x_0 component of 2D vector
    constexpr auto x0 = mito::functions::component<coordinates_t, 0>;
    // the function extracting the x_1 component of a 2D vector
    constexpr auto x1 = mito::functions::component<coordinates_t, 1>;

    // a scalar field
    constexpr auto f = mito::fields::field(cos(x0 * x1));

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ 0.0, 0.0 });

    // check value of field at {x}
    static_assert(f(x) == 1.0);

    // the function returning the constant e0 unit vector in 2D
    constexpr auto e0 = mito::functions::constant<coordinates_t>(mito::e_0<2>);

    // the function returning the constant e1 unit vector in 2D
    constexpr auto e1 = mito::functions::constant<coordinates_t>(mito::e_1<2>);

    // a vector field
    constexpr auto g = mito::fields::field(cos(x0 * x1) * (e0 + e1));

    // check value of field at {x}
    static_assert(g(x) == mito::vector_t<2>{ 1.0, 1.0 });
}


// end of file
