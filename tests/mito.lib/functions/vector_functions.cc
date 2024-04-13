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
    static_assert(1.0 == x0_0);
    static_assert(0.0 == x0_1);

    // the partial derivatives of x1
    constexpr auto x1_0 = mito::functions::derivative<0>(x1);
    constexpr auto x1_1 = mito::functions::derivative<1>(x1);
    // sanity check
    static_assert(0.0 == x1_0);
    static_assert(1.0 == x1_1);
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
}


// end of file
