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


TEST(Derivatives, VectorSum)
{
    // a 2D vector
    constexpr auto x = mito::vector_t<2>{ 0.1, 1.0 };

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::x<0, 2>;
    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;

    // check result
    static_assert(1.1 == (x0 + x1)(x));

    // the partial derivatives of x0 + x1 wrt to x0
    constexpr auto x0px1_0 = mito::functions::derivative<0>(x0 + x1);
    // check result
    static_assert(1.0 == x0px1_0(x));

    // the partial derivatives of x0 + x1 wrt to x1
    constexpr auto x0px1_1 = mito::functions::derivative<1>(x0 + x1);
    // check result
    static_assert(1.0 == x0px1_1(x));
}


// end of file