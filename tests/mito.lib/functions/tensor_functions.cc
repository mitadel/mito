// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


TEST(TensorFunctions, Algebra)
{
    // the function extracting the x_0 component of a 3D vector
    constexpr auto x0 = mito::functions::x<0, 3>;
    // the function extracting the x_1 component of a 3D vector
    constexpr auto x1 = mito::functions::x<1, 3>;
    // the function extracting the x_2 component of a 3D vector
    constexpr auto x2 = mito::functions::x<2, 3>;
    // the cosine function
    constexpr auto cos = mito::functions::cos;

    // a scalar function
    constexpr auto f = x1 * x2 * cos(x0);

    // a point in space
    constexpr auto x = 2.0 * mito::e_1<3> + mito::e_2<3>;

    // evaluating {f} on {x} yields 2.0
    static_assert(f(x) == 2.0);

    // another scalar function
    auto g = f * f;

    // a tensor function (obtained by linear combination of scalar functions and tensors)
    constexpr auto B1 = f * mito::e_12<3> + g * mito::e_22<3>;

    // a tensor function
    constexpr auto B2 = mito::functions::constant<mito::vector_t<3>>(mito::e_12<3>);

    // add the two tensor functions
    constexpr auto B = B1 + B2;

    // the tensor: (f(x) + 1) * e_12 + g(x) * e_21
    constexpr auto A = 3.0 * mito::e_12<3> + 4.0 * mito::e_22<3>;

    // evaluating {B} on {x} yields {A}
    static_assert(B(x) == A);
}