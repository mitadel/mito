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
// pi fourth
constexpr auto pi_fourth = std::numbers::pi / 4.0;


TEST(Identities, DivGrad)
{
    // the field returning the constant {e_0} unit vector in 2D
    constexpr auto e0 = mito::fields::uniform_field<coordinates_t>(e_0);

    // the field returning the constant {e_1} unit vector in 2D
    constexpr auto e1 = mito::fields::uniform_field<coordinates_t>(e_1);

    // the sine function
    constexpr auto sin = mito::functions::sin;

    // the cosine function
    constexpr auto cos = mito::functions::cos;

    // the function extracting the x_0 component of a 2D vector
    constexpr auto x0 = mito::functions::component<coordinates_t, 0>;

    // the function extracting the x_1 component of a 2D vector
    constexpr auto x1 = mito::functions::component<coordinates_t, 1>;

    // a vector field
    constexpr auto f = sin(x0 * x1) * e0 + cos(x0 * x1) * e1;

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ pi_sixth, pi_fourth });

    // the divergence of the gradient transposed of {f}
    constexpr auto div_grad_T =
        mito::fields::divergence(mito::fields::transpose(mito::fields::gradient(f)));

    // the gradient of the divergence of {f}
    constexpr auto grad_div = mito::fields::gradient(mito::fields::divergence(f));

    // check result
    static_assert(div_grad_T(x) == grad_div(x));
}
