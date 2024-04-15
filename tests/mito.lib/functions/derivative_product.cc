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
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a cosine function
    constexpr auto cos = mito::functions::cos;
    // the functions product
    constexpr auto product = sin * cos;
    // sanity check
    EXPECT_DOUBLE_EQ(sin(pi_fourth) * cos(pi_fourth), product(pi_fourth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {cos}
    constexpr auto cos_i = mito::functions::derivative(cos);
    // the first derivative of the product
    constexpr auto product_i = mito::functions::derivative(product);
    // sanity check
    EXPECT_DOUBLE_EQ(
        sin(pi_fourth) * cos_i(pi_fourth) + sin_i(pi_fourth) * cos(pi_fourth),
        product_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {cos}
    constexpr auto cos_ii = mito::functions::derivative(cos_i);
    // the second derivative of the product
    constexpr auto product_ii = mito::functions::derivative(product_i);
    // sanity check
    EXPECT_DOUBLE_EQ(
        sin(pi_fourth) * cos_ii(pi_fourth) + sin_ii(pi_fourth) * cos(pi_fourth)
            + 2.0 * sin_i(pi_fourth) * cos_i(pi_fourth),
        product_ii(pi_fourth));
}


TEST(Derivatives, Reciprocal)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a cosine function
    constexpr auto cos = mito::functions::cos;
    // the reciprocal of cos
    constexpr auto sec = mito::functions::sec;
    // sanity check
    EXPECT_DOUBLE_EQ(1.0 / cos(pi_fourth), sec(pi_fourth));

    // a tangent function
    constexpr auto tan = mito::functions::tan;
    // the first derivative of the secant
    constexpr auto sec_i = mito::functions::derivative(sec);
    // sanity check
    EXPECT_DOUBLE_EQ(tan(pi_fourth) * sec(pi_fourth), sec_i(pi_fourth));

    // the first derivative of the tangent
    constexpr auto tan_i = mito::functions::derivative(tan);
    // sanity check
    EXPECT_DOUBLE_EQ(sec(pi_fourth) * sec(pi_fourth), tan_i(pi_fourth));
}


TEST(Derivatives, Power)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // power of two function
    constexpr auto pow2 = mito::functions::pow<2>;
    // sanity check
    static_assert(pi_fourth * pi_fourth == pow2(pi_fourth));
    EXPECT_DOUBLE_EQ(pi_fourth * pi_fourth, pow2(pi_fourth));

    // the first derivative of {pow2}
    constexpr auto pow2_i = mito::functions::derivative(pow2);
    // linear function
    constexpr auto pow1 = mito::functions::pow<1>;
    // sanity check
    static_assert(2.0 * pow1(pi_fourth) == pow2_i(pi_fourth));
    EXPECT_DOUBLE_EQ(2.0 * pow1(pi_fourth), pow2_i(pi_fourth));

    // the second derivative of {pow2}
    constexpr auto pow2_ii = mito::functions::derivative(pow2_i);
    // sanity check
    static_assert(2.0 == pow2_ii(pi_fourth));
    EXPECT_DOUBLE_EQ(2.0, pow2_ii(pi_fourth));
}


// end of file
