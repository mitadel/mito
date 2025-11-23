// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fields.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


// the basis for vectors in 2D
static constexpr auto e_0 = mito::tensor::e_0<2>;
static constexpr auto e_1 = mito::tensor::e_1<2>;

// the basis for 2x3 tensors
static constexpr auto e_00 = mito::tensor::e_00<2, 3>;
static constexpr auto e_01 = mito::tensor::e_01<2, 3>;
static constexpr auto e_02 = mito::tensor::e_02<2, 3>;
static constexpr auto e_10 = mito::tensor::e_10<2, 3>;
static constexpr auto e_11 = mito::tensor::e_11<2, 3>;
static constexpr auto e_12 = mito::tensor::e_12<2, 3>;

// pi sixth
constexpr auto pi_sixth = std::numbers::pi / 6.0;
// pi fourth
constexpr auto pi_fourth = std::numbers::pi / 4.0;


TEST(Gradient, NonSquare)
{
    // the field returning the constant {e_0} unit vector in 2D
    constexpr auto e0 = mito::fields::uniform_field<coordinates_t>(e_0);

    // the field returning the constant {e_1} unit vector in 2D
    constexpr auto e1 = mito::fields::uniform_field<coordinates_t>(e_1);

    // the sine function
    constexpr auto sin = mito::functions::sin;

    // the cosine function
    constexpr auto cos = mito::functions::cos;

    // the function extracting the x_0 component of {coordinates_t}
    constexpr auto x0 = mito::functions::component<coordinates_t, 0>;

    // the function extracting the x_1 component of {coordinates_t}
    constexpr auto x1 = mito::functions::component<coordinates_t, 1>;

    // the function extracting the x_1 component of {coordinates_t}
    constexpr auto x2 = mito::functions::component<coordinates_t, 2>;

    // a vector field
    constexpr auto f = (sin(x0 * x1) + x2) * e0 + (cos(x0 * x1) - x2) * e1;

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ pi_sixth, pi_fourth, 1.0 });

    // the gradient of {f}
    constexpr auto gradient = mito::fields::gradient(f);

    // create a channel
    journal::info_t channel("tests.gradient_non_square");

    // report on the gradient at {x}
    channel << "Gradient at x = " << x << " is " << gradient(x) << journal::endl;

    // check result
    static_assert(
        gradient(x)
        == (cos(x0 * x1) * x1 * e_00 + cos(x0 * x1) * x0 * e_01 - sin(x0 * x1) * x1 * e_10
            - sin(x0 * x1) * x0 * e_11 + e_02 - e_12)(x));
}
