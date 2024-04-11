// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>


// pi
using std::numbers::pi;


// TOFIX: make {constexpr}

TEST(Derivatives, HigherOrder)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::math::sin();
    // a cosine function
    constexpr auto cos = mito::math::cos();

    // the first derivative of {sin}
    auto sin_i = mito::math::derivative(sin);
    // check that it is equal to {cos}
    EXPECT_DOUBLE_EQ(cos(pi_fourth), sin_i(pi_fourth));

    // the second derivative of {sin}
    auto sin_ii = mito::math::derivative(sin_i);
    // check that it is equal to -{sin}
    EXPECT_DOUBLE_EQ(-sin(pi_fourth), sin_ii(pi_fourth));

    // the third derivative of {sin}
    auto sin_iii = mito::math::derivative(sin_ii);
    // check that it is equal to -{cos}
    EXPECT_DOUBLE_EQ(-cos(pi_fourth), sin_iii(pi_fourth));

    // the fourth derivative of {sin}
    auto sin_iiii = mito::math::derivative(sin_iii);
    // check that it is equal to {sin}
    EXPECT_DOUBLE_EQ(sin(pi_fourth), sin_iiii(pi_fourth));
}


TEST(Derivatives, Sum)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::math::sin();
    // a cosine function
    constexpr auto cos = mito::math::cos();
    // the functions sum
    constexpr auto sum = sin + cos;
    // check that it is equal to {sin + cos}
    EXPECT_DOUBLE_EQ(sin(pi_fourth) + cos(pi_fourth), sum(pi_fourth));

    // the first derivative of {sin}
    auto sin_i = mito::math::derivative(sin);
    // the first derivative of {cos}
    auto cos_i = mito::math::derivative(cos);
    // the first derivative of the sum
    auto sum_i = mito::math::derivative(sum);
    // check that it is equal to {cos}
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth) + cos_i(pi_fourth), sum_i(pi_fourth));

    // the second derivative of {sin}
    auto sin_ii = mito::math::derivative(sin_i);
    // the second derivative of {cos}
    auto cos_ii = mito::math::derivative(cos_i);
    // the second derivative of the sum
    auto sum_ii = mito::math::derivative(sum_i);
    // check that it is equal to {sin_ii + cos_ii}
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth) + cos_ii(pi_fourth), sum_ii(pi_fourth));
}


TEST(Derivatives, FunctionTimesScalar)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::math::sin();
    // a scalar
    constexpr auto a = 0.1;
    // the scalar times function product
    constexpr auto a_sin = a * sin;
    // the function times scalar product
    constexpr auto sin_a = sin * a;
    // sanity checks
    EXPECT_DOUBLE_EQ(a * sin(pi_fourth), a_sin(pi_fourth));
    EXPECT_DOUBLE_EQ(a * sin(pi_fourth), sin_a(pi_fourth));

    // the first derivative of {sin}
    auto sin_i = mito::math::derivative(sin);
    // the first derivative of {a_sin}
    auto a_sin_i = mito::math::derivative(a_sin);
    // the first derivative of {a_sin}
    auto sin_a_i = mito::math::derivative(sin_a);
    // sanity checks
    EXPECT_DOUBLE_EQ(a * sin_i(pi_fourth), a_sin_i(pi_fourth));
    EXPECT_DOUBLE_EQ(a * sin_i(pi_fourth), sin_a_i(pi_fourth));

    // the second derivative of {sin}
    auto sin_ii = mito::math::derivative(sin_i);
    // the second derivative of {a_sin}
    auto a_sin_ii = mito::math::derivative(a_sin_i);
    // the first derivative of {a_sin}
    auto sin_a_ii = mito::math::derivative(sin_a_i);
    // sanity checks
    EXPECT_DOUBLE_EQ(a * sin_ii(pi_fourth), a_sin_ii(pi_fourth));
    EXPECT_DOUBLE_EQ(a * sin_ii(pi_fourth), sin_a_ii(pi_fourth));
}


TEST(Derivatives, Subtraction)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::math::sin();
    // a cosine function
    constexpr auto cos = mito::math::cos();
    // the functions sub
    constexpr auto sub = sin - cos;
    // check that it is equal to {sin + cos}
    EXPECT_DOUBLE_EQ(sin(pi_fourth) - cos(pi_fourth), sub(pi_fourth));

    // the first derivative of {sin}
    auto sin_i = mito::math::derivative(sin);
    // the first derivative of {cos}
    auto cos_i = mito::math::derivative(cos);
    // the first derivative of the sub
    auto sub_i = mito::math::derivative(sub);
    // check that it is equal to {cos}
    EXPECT_DOUBLE_EQ(sin_i(pi_fourth) - cos_i(pi_fourth), sub_i(pi_fourth));

    // the second derivative of {sin}
    auto sin_ii = mito::math::derivative(sin_i);
    // the second derivative of {cos}
    auto cos_ii = mito::math::derivative(cos_i);
    // the second derivative of the sub
    auto sub_ii = mito::math::derivative(sub_i);
    // check that it is equal to {sin_ii + cos_ii}
    EXPECT_DOUBLE_EQ(sin_ii(pi_fourth) - cos_ii(pi_fourth), sub_ii(pi_fourth));
}