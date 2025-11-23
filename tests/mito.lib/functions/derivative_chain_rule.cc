// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;
// mito scalars
using mito::tensor::scalar_t;


TEST(Derivatives, Scalar)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // sine function
    constexpr auto sin = mito::functions::sin;
    // power of two function
    constexpr auto pow2 = mito::functions::pow<2>;

    // the function sin(pow<2>)
    constexpr auto sin_pow2 = sin(pow2);
    // check result
    static_assert(sin(pow2(pi_sixth)) == sin_pow2(pi_sixth));

    // the derivative of sin(pow<2>)
    constexpr auto sin_pow2_i = mito::functions::derivative(sin_pow2);
    // cosine function
    constexpr auto cos = mito::functions::cos;
    // power of one function
    constexpr auto pow1 = mito::functions::pow<1>;
    // check result
    static_assert(2.0 * cos(pow2(pi_sixth)) * pow1(pi_sixth) == sin_pow2_i(pi_sixth));

    // the function pow<2>(sin)
    constexpr auto pow2_sin = pow2(sin);
    // check result
    static_assert(pow2(sin(pi_sixth)) == pow2_sin(pi_sixth));

    // the derivative of sin(pow<2>)
    constexpr auto pow2_sin_i = mito::functions::derivative(pow2_sin);
    // check result
    static_assert(2.0 * pow1(sin(pi_sixth)) * cos(pi_sixth) == pow2_sin_i(pi_sixth));
}


// the coordinates type
using coordinates_type_2D = mito::tensor::vector_t<2>;
using coordinates_type_3D = mito::tensor::vector_t<3>;


TEST(Derivatives, Vector)
{
    // components 2D
    constexpr auto a = mito::functions::component<coordinates_type_2D, 0>;
    constexpr auto b = mito::functions::component<coordinates_type_2D, 1>;

    // a vector function R^2 -> R^3 (internal in the composition)
    constexpr auto g = a * b * mito::tensor::e<1, 3> + (a + b) * mito::tensor::e<2, 3>;

    // components 3D
    constexpr auto x = mito::functions::component<coordinates_type_3D, 0>;
    constexpr auto y = mito::functions::component<coordinates_type_3D, 1>;
    constexpr auto z = mito::functions::component<coordinates_type_3D, 2>;

    // a vector function R^3 -> R^3 (external in the composition)
    constexpr auto f =
        x * mito::tensor::e<0, 3> + y * mito::tensor::e<1, 3> + z * mito::tensor::e<2, 3>;

    // the composition of f and g (R^2 -> R^3)
    constexpr auto h = f(g);

    // pick a point
    constexpr auto p = coordinates_type_2D{ 1.0, 2.0 };

    // f(g) = a * b * mito::tensor::e<1, 3> + (a + b) * mito::tensor::e<2, 3>
    static_assert(
        h(p) == p[0] * p[1] * mito::tensor::e<1, 3> + (p[0] + p[1]) * mito::tensor::e<2, 3>);

    // the derivative of h
    constexpr auto h_0 = mito::functions::derivative<0>(h);
    constexpr auto h_1 = mito::functions::derivative<1>(h);

    // check the result
    static_assert(h_0(p) == p[1] * mito::tensor::e<1, 3> + mito::tensor::e<2, 3>);
    static_assert(h_1(p) == p[0] * mito::tensor::e<1, 3> + mito::tensor::e<2, 3>);
}


// end of file
