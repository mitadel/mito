// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fields.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


// the basis for vectors in 2D
static constexpr auto e_0 = mito::tensor::e_0<2>;
static constexpr auto e_1 = mito::tensor::e_1<2>;

// pi sixth
constexpr auto pi_sixth = std::numbers::pi / 6.0;


TEST(Fields, Gradient)
{
    // the sine function
    constexpr auto sin = mito::functions::sin;
    // the cosine function
    constexpr auto cos = mito::functions::cos;
    // the function extracting the x_0 component of a 2D vector
    constexpr auto x0 = mito::functions::component<coordinates_t, 0>;
    // the function extracting the x_1 component of a 2D vector
    constexpr auto x1 = mito::functions::component<coordinates_t, 1>;

    // a scalar field
    constexpr auto f = mito::fields::field(sin(x0 * x1));

    // the gradient of {f}
    constexpr auto gradient = mito::fields::gradient(f);

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ pi_sixth, 1.0 });

    // check result
    static_assert(gradient(x) == (cos(x0 * x1) * x1 * e_0 + cos(x0 * x1) * x0 * e_1)(x));

    // the laplacian (divergence of gradient)
    constexpr auto laplacian = mito::fields::divergence(gradient);

    // check result
    static_assert(laplacian(x) == (-sin(x0 * x1) * x1 * x1 - sin(x0 * x1) * x0 * x0)(x));
}
