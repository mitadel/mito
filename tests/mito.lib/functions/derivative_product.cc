// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;
// mito scalars
using mito::scalar_t;


TEST(Derivatives, Product)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a cosine function
    constexpr auto cos = mito::functions::cos;
    // the functions product
    constexpr auto product = sin * cos;
    // check result
    static_assert(sin(pi_sixth) * cos(pi_sixth) == product(pi_sixth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {cos}
    constexpr auto cos_i = mito::functions::derivative(cos);
    // the first derivative of the product
    constexpr auto product_i = mito::functions::derivative(product);
    // check result
    static_assert(
        sin(pi_sixth) * cos_i(pi_sixth) + sin_i(pi_sixth) * cos(pi_sixth) == product_i(pi_sixth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {cos}
    constexpr auto cos_ii = mito::functions::derivative(cos_i);
    // the second derivative of the product
    constexpr auto product_ii = mito::functions::derivative(product_i);
    // check result
    static_assert(
        sin(pi_sixth) * cos_ii(pi_sixth) + sin_ii(pi_sixth) * cos(pi_sixth)
            + 2.0 * sin_i(pi_sixth) * cos_i(pi_sixth)
        == product_ii(pi_sixth));
}


TEST(Derivatives, Reciprocal)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a cosine function
    constexpr auto cos = mito::functions::cos;
    // the secant function (reciprocal of cos)
    constexpr auto sec = 1.0 / cos;

    // a tangent function
    constexpr auto tan = mito::functions::tan;
    // the first derivative of the secant
    constexpr auto sec_i = mito::functions::derivative(sec);
    // check result
    static_assert(tan(pi_fourth) * sec(pi_fourth) == sec_i(pi_fourth));

    // the first derivative of the tangent
    constexpr auto tan_i = mito::functions::derivative(tan);
    // check result
    static_assert(sec(pi_fourth) * sec(pi_fourth) == tan_i(pi_fourth));
}


TEST(Derivatives, Power)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // power of two function
    constexpr auto pow2 = mito::functions::pow<2>;
    // check result
    static_assert(pi_sixth * pi_sixth == pow2(pi_sixth));

    // the first derivative of {pow2}
    constexpr auto pow2_i = mito::functions::derivative(pow2);
    // linear function
    constexpr auto pow1 = mito::functions::pow<1>;
    // check result
    static_assert(2.0 * pow1(pi_sixth) == pow2_i(pi_sixth));

    // the second derivative of {pow2}
    constexpr auto pow2_ii = mito::functions::derivative(pow2_i);
    // check result
    static_assert(2.0 == pow2_ii(pi_sixth));
}


TEST(Derivatives, ScalarProduct)
{
    // a 2D vector
    constexpr auto x = mito::vector_t<2>{ 0.1, 1.0 };

    // a 2D vector
    constexpr auto a = mito::vector_t<2>{ -1.0, 1.0 };

    // the function returning the constant e0 unit vector in 2D
    constexpr auto e0 = mito::functions::constant<mito::vector_t<2>>(mito::e_0<2>);

    // the function returning the constant e1 unit vector in 2D
    constexpr auto e1 = mito::functions::constant<mito::vector_t<2>>(mito::e_1<2>);

    // the function extracting the x_0 component of a 2D vector
    constexpr auto x0 = mito::functions::x<0, 2>;

    // the function extracting the x_1 component of a 2D vector
    constexpr auto x1 = mito::functions::x<1, 2>;

    // a function {f}
    constexpr auto f = a * (x0 * e0 + x1 * e1);

    // the derivative of f with respect to x0
    constexpr auto f_0 = mito::functions::derivative<0>(f);
    constexpr auto f_1 = mito::functions::derivative<1>(f);

    // check result
    static_assert(a * e0(x) == f_0(x));
    static_assert(a * e1(x) == f_1(x));
}


// end of file
