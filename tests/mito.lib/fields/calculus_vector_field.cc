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

// the basis for tensors in 2D
static constexpr auto e_00 = mito::tensor::e_00<2>;
static constexpr auto e_01 = mito::tensor::e_01<2>;
static constexpr auto e_10 = mito::tensor::e_10<2>;
static constexpr auto e_11 = mito::tensor::e_11<2>;

// pi sixth
constexpr auto pi_sixth = std::numbers::pi / 6.0;
// pi fourth
constexpr auto pi_fourth = std::numbers::pi / 4.0;


TEST(Laplacian, VectorFields)
{
    // the sine function
    constexpr auto sin = mito::functions::sin;

    // the cosine function
    constexpr auto cos = mito::functions::cos;

    // the function extracting the x_0 component of a 2D vector
    constexpr auto x0 = mito::functions::component<coordinates_t, 0>;

    // the function extracting the x_1 component of a 2D vector
    constexpr auto x1 = mito::functions::component<coordinates_t, 1>;

    // a vector field
    constexpr auto g = sin(x0 * x1) * e_0 + cos(x0 * x1) * e_1;

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ pi_sixth, pi_fourth });

    // the gradient of {g}
    constexpr auto gradient = mito::fields::gradient(g);

    // check result
    static_assert(
        gradient(x)
        == (cos(x0 * x1) * x1 * e_00 + cos(x0 * x1) * x0 * e_01 - sin(x0 * x1) * x1 * e_10
            - sin(x0 * x1) * x0 * e_11)(x));

    // the laplacian (divergence of gradient)
    constexpr auto laplacian = mito::fields::divergence(gradient);

    // check result
    static_assert(laplacian(x) == (-(x0 * x0 + x1 * x1) * g)(x));
}
