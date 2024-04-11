// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


TEST(Math, TensorFunctions)
{
    // a scalar function
    constexpr auto f = mito::math::function(
        [](const mito::vector_t<3> & x) -> mito::scalar_t { return x[1] * x[2] * std::cos(x[0]); });

    // a point in space
    constexpr auto x = 2.0 * mito::e_1<3> + mito::e_2<3>;

    // evaluating {f} on {x} yields 2.0
    static_assert(f(x) == 2.0);

    // another scalar function
    auto g = f * f;

    // a tensor function (obtained by linear combination of scalar functions and tensors)
    constexpr auto B1 = f * mito::e_12<3> + g * mito::e_22<3>;

    // a tensor function
    constexpr auto B2 = mito::math::function(
        [](const mito::vector_t<3> &) -> mito::matrix_t<3> { return mito::e_12<3>; });

    // add the two tensor functions
    constexpr auto B = B1 + B2;

    // the tensor: (f(x) + 1) * e_12 + g(x) * e_21
    constexpr auto A = 3.0 * mito::e_12<3> + 4.0 * mito::e_22<3>;

    // evaluating {B} on {x} yields {A}
    static_assert(B(x) == A);
}