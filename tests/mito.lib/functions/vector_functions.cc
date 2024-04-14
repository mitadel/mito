// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


TEST(VectorFunctions, Components)
{
    // a 2D vector
    constexpr auto x = mito::vector_t<2>{ 0.1, 1.0 };

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::x<0, 2>;
    // sanity check
    static_assert(0.1 == x0(x));

    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;
    // sanity check
    static_assert(1.0 == x1(x));

    // the partial derivatives of x0
    constexpr auto x0_0 = mito::functions::derivative<0>(x0);
    constexpr auto x0_1 = mito::functions::derivative<1>(x0);
    // sanity check
    static_assert(1.0 == x0_0(x));
    static_assert(0.0 == x0_1(x));

    // the partial derivatives of x1
    constexpr auto x1_0 = mito::functions::derivative<0>(x1);
    constexpr auto x1_1 = mito::functions::derivative<1>(x1);
    // sanity check
    static_assert(0.0 == x1_0(x));
    static_assert(1.0 == x1_1(x));

    // the second partial derivatives of x0
    constexpr auto x0_00 = mito::functions::derivative<0>(x0_0);
    constexpr auto x0_01 = mito::functions::derivative<1>(x0_0);
    constexpr auto x0_10 = mito::functions::derivative<0>(x0_1);
    constexpr auto x0_11 = mito::functions::derivative<1>(x0_1);
    // sanity check
    static_assert(0.0 == x0_00(x));
    static_assert(0.0 == x0_01(x));
    static_assert(0.0 == x0_10(x));
    static_assert(0.0 == x0_11(x));

    // the second partial derivatives of x1
    constexpr auto x1_00 = mito::functions::derivative<0>(x1_0);
    constexpr auto x1_01 = mito::functions::derivative<1>(x1_0);
    constexpr auto x1_10 = mito::functions::derivative<0>(x1_1);
    constexpr auto x1_11 = mito::functions::derivative<1>(x1_1);
    // sanity check
    static_assert(0.0 == x1_00(x));
    static_assert(0.0 == x1_01(x));
    static_assert(0.0 == x1_10(x));
    static_assert(0.0 == x1_11(x));
}


TEST(VectorFunctions, Sum)
{
    // a 2D vector
    constexpr auto x = mito::vector_t<2>{ 0.1, 1.0 };

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::x<0, 2>;
    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;

    // sanity check
    static_assert(1.1 == (x0 + x1)(x));

    // the partial derivatives of x0 + x1 wrt to x0
    constexpr auto x0px1_0 = mito::functions::derivative<0>(x0 + x1);
    // sanity check
    static_assert(1.0 == x0px1_0(x));

    // the partial derivatives of x0 + x1 wrt to x1
    constexpr auto x0px1_1 = mito::functions::derivative<1>(x0 + x1);
    // sanity check
    static_assert(1.0 == x0px1_1(x));
}


TEST(VectorFunctions, Product)
{
    // a 2D vector
    constexpr auto x = mito::vector_t<2>{ 0.1, 1.0 };

    // the function returning the constant e0 unit vector in 2D
    constexpr auto e0 = mito::functions::unit<0, 2>;

    // sanity check
    static_assert(1.0 == (e0 * e0)(x));

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::x<0, 2>;
    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;

    // sanity check
    static_assert(0.1 == (x0 * x1)(x));
}


TEST(VectorFunctions, Sin)
{
    // a 2D vector
    constexpr auto x = mito::vector_t<2>{ 0.1, 1.0 };

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::x<0, 2>;
    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;

    // sin(x0 * x1)
    constexpr auto sin = mito::functions::sin(x0 * x1);

    // the partial derivative of sin(x0 * x1) wrt to x0
    constexpr auto sin_0 = mito::functions::derivative<0>(sin);
    // the partial derivative of sin(x0 * x1) wrt to x1
    constexpr auto sin_1 = mito::functions::derivative<1>(sin);

    // cos(x0 * x1)
    constexpr auto cos = mito::functions::cos(x0 * x1);

    // check result
    static_assert((cos * x1)(x) == sin_0(x));
    static_assert((cos * x0)(x) == sin_1(x));
}

// end of file
