// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;


TEST(Derivatives, HigherOrder)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a cosine function
    constexpr auto cos = mito::functions::cos;

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // check that it is equal to {cos}
    EXPECT_DOUBLE_EQ(cos(pi_fourth), sin_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // check that it is equal to -{sin}
    EXPECT_DOUBLE_EQ(-sin(pi_fourth), sin_ii(pi_fourth));

    // the third derivative of {sin}
    constexpr auto sin_iii = mito::functions::derivative(sin_ii);
    // check that it is equal to -{cos}
    EXPECT_DOUBLE_EQ(-cos(pi_fourth), sin_iii(pi_fourth));

    // the fourth derivative of {sin}
    constexpr auto sin_iiii = mito::functions::derivative(sin_iii);
    // check that it is equal to {sin}
    EXPECT_DOUBLE_EQ(sin(pi_fourth), sin_iiii(pi_fourth));
}


TEST(Derivatives, Sum)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a cosine function
    constexpr auto cos = mito::functions::cos;
    // the functions sum
    constexpr auto sum = sin + cos;
    // check that it is equal to {sin + cos}
    EXPECT_DOUBLE_EQ(sin(pi_fourth) + cos(pi_fourth), sum(pi_fourth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {cos}
    constexpr auto cos_i = mito::functions::derivative(cos);
    // the first derivative of the sum
    constexpr auto sum_i = mito::functions::derivative(sum);
    // check that it is equal to {cos}
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth) + cos_i(pi_fourth), sum_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {cos}
    constexpr auto cos_ii = mito::functions::derivative(cos_i);
    // the second derivative of the sum
    constexpr auto sum_ii = mito::functions::derivative(sum_i);
    // check that it is equal to {sin_ii + cos_ii}
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth) + cos_ii(pi_fourth), sum_ii(pi_fourth));
}


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


TEST(Derivatives, Subtraction)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a cosine function
    constexpr auto cos = mito::functions::cos;
    // the functions sub
    constexpr auto sub = sin - cos;
    // check that it is equal to {sin + cos}
    EXPECT_DOUBLE_EQ(sin(pi_fourth) - cos(pi_fourth), sub(pi_fourth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {cos}
    constexpr auto cos_i = mito::functions::derivative(cos);
    // the first derivative of the sub
    constexpr auto sub_i = mito::functions::derivative(sub);
    // check that it is equal to {cos}
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth) - cos_i(pi_fourth), sub_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {cos}
    constexpr auto cos_ii = mito::functions::derivative(cos_i);
    // the second derivative of the sub
    constexpr auto sub_ii = mito::functions::derivative(sub_i);
    // check that it is equal to {sin_ii + cos_ii}
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth) - cos_ii(pi_fourth), sub_ii(pi_fourth));
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


TEST(Derivatives, Zero)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // zero function
    constexpr auto zero = mito::functions::zero;
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
    constexpr auto one = mito::functions::one;
    // sanity check
    EXPECT_DOUBLE_EQ(1.0, one(pi_fourth));

    // the first derivative of {one}
    constexpr auto one_i = mito::functions::derivative(one);
    // zero function
    constexpr auto zero = mito::functions::zero;
    // sanity check
    EXPECT_DOUBLE_EQ(zero(pi_fourth), one_i(pi_fourth));

    // the second derivative of {one}
    constexpr auto one_ii = mito::functions::derivative(one_i);
    // sanity check
    EXPECT_DOUBLE_EQ(zero(pi_fourth), one_ii(pi_fourth));
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


TEST(Derivatives, Composition)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // sine function
    constexpr auto sin = mito::functions::sin;
    // power of two function
    constexpr auto pow2 = mito::functions::pow<2>;

    // the function sin(pow<2>)
    constexpr auto sin_pow2 = sin(pow2);
    // sanity check
    EXPECT_DOUBLE_EQ(sin(pow2(pi_fourth)), sin_pow2(pi_fourth));

    // the derivative of sin(pow<2>)
    constexpr auto sin_pow2_i = mito::functions::derivative(sin_pow2);
    // cosine function
    constexpr auto cos = mito::functions::cos;
    // power of one function
    constexpr auto pow1 = mito::functions::pow<1>;
    // sanity check
    EXPECT_DOUBLE_EQ(2.0 * cos(pow2(pi_fourth)) * pow1(pi_fourth), sin_pow2_i(pi_fourth));

    // the function pow<2>(sin)
    constexpr auto pow2_sin = pow2(sin);
    // sanity check
    EXPECT_DOUBLE_EQ(pow2(sin(pi_fourth)), pow2_sin(pi_fourth));

    // the derivative of sin(pow<2>)
    constexpr auto pow2_sin_i = mito::functions::derivative(pow2_sin);
    // sanity check
    EXPECT_DOUBLE_EQ(2.0 * pow1(sin(pi_fourth)) * cos(pi_fourth), pow2_sin_i(pi_fourth));
}