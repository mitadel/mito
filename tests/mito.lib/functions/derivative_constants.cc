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


TEST(Derivatives, FunctionPlusConstant)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the constant plus the function
    constexpr auto a_sin = a + sin;
    // the function plus the constant
    constexpr auto sin_a = sin + a;
    // sanity checks
    EXPECT_DOUBLE_EQ(a + sin(pi_fourth), a_sin(pi_fourth));
    EXPECT_DOUBLE_EQ(a + sin(pi_fourth), sin_a(pi_fourth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto a_sin_i = mito::functions::derivative(a_sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // sanity checks
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth), a_sin_i(pi_fourth));
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth), sin_a_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {a_sin}
    constexpr auto a_sin_ii = mito::functions::derivative(a_sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // sanity checks
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth), a_sin_ii(pi_fourth));
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth), sin_a_ii(pi_fourth));
}


TEST(Derivatives, FunctionTimesConstant)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the constant times function product
    constexpr auto a_sin = a * sin;
    // the function times constant product
    constexpr auto sin_a = sin * a;
    // sanity checks
    EXPECT_DOUBLE_EQ(a * sin(pi_fourth), a_sin(pi_fourth));
    EXPECT_DOUBLE_EQ(a * sin(pi_fourth), sin_a(pi_fourth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto a_sin_i = mito::functions::derivative(a_sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // sanity checks
    EXPECT_DOUBLE_EQ(a * sin_i(pi_fourth), a_sin_i(pi_fourth));
    EXPECT_DOUBLE_EQ(a * sin_i(pi_fourth), sin_a_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {a_sin}
    constexpr auto a_sin_ii = mito::functions::derivative(a_sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // sanity checks
    EXPECT_DOUBLE_EQ(a * sin_ii(pi_fourth), a_sin_ii(pi_fourth));
    EXPECT_DOUBLE_EQ(a * sin_ii(pi_fourth), sin_a_ii(pi_fourth));
}


TEST(Derivatives, FunctionDividedByConstant)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the function times constant product
    constexpr auto sin_a = sin / a;
    // sanity check
    EXPECT_DOUBLE_EQ(sin(pi_fourth) / a, sin_a(pi_fourth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // sanity check
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth) / a, sin_a_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // sanity check
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth) / a, sin_a_ii(pi_fourth));
}


TEST(Derivatives, FunctionMinusConstant)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the constant minus the function
    constexpr auto a_sin = a - sin;
    // the function minus the constant
    constexpr auto sin_a = sin - a;
    // sanity checks
    EXPECT_DOUBLE_EQ(a - sin(pi_fourth), a_sin(pi_fourth));
    EXPECT_DOUBLE_EQ(sin(pi_fourth) - a, sin_a(pi_fourth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto a_sin_i = mito::functions::derivative(a_sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // sanity checks
    EXPECT_DOUBLE_EQ(-sin_i(pi_fourth), a_sin_i(pi_fourth));
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth), sin_a_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {a_sin}
    constexpr auto a_sin_ii = mito::functions::derivative(a_sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // sanity checks
    EXPECT_DOUBLE_EQ(-sin_ii(pi_fourth), a_sin_ii(pi_fourth));
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth), sin_a_ii(pi_fourth));
}


TEST(Derivatives, Zero)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // zero function
    constexpr auto zero = mito::functions::zero<scalar_t>;
    // sanity check
    EXPECT_DOUBLE_EQ(0.0, zero(pi_fourth));

    // the first derivative of {zero}
    constexpr auto zero_i = mito::functions::derivative(zero);
    // sanity check
    EXPECT_DOUBLE_EQ(zero(pi_fourth), zero_i(pi_fourth));

    // the second derivative of {zero}
    constexpr auto zero_ii = mito::functions::derivative(zero_i);
    // sanity check
    EXPECT_DOUBLE_EQ(zero(pi_fourth), zero_ii(pi_fourth));
}


TEST(Derivatives, One)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // identity function
    constexpr auto one = mito::functions::one<scalar_t>;
    // sanity check
    EXPECT_DOUBLE_EQ(1.0, one(pi_fourth));

    // the first derivative of {one}
    constexpr auto one_i = mito::functions::derivative(one);
    // zero function
    constexpr auto zero = mito::functions::zero<scalar_t>;
    // sanity check
    EXPECT_DOUBLE_EQ(zero(pi_fourth), one_i(pi_fourth));

    // the second derivative of {one}
    constexpr auto one_ii = mito::functions::derivative(one_i);
    // sanity check
    EXPECT_DOUBLE_EQ(zero(pi_fourth), one_ii(pi_fourth));
}


// end of file
