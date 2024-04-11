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
    EXPECT_DOUBLE_EQ(0.1, x0(x));

    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;
    // sanity check
    EXPECT_DOUBLE_EQ(1.0, x1(x));
}

// end of file
