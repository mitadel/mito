// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/tensor.h>


TEST(Tensor, OneForms)
{
    // the Euclidean metric in 3D space
    constexpr auto metric = mito::tensor::identity<3>;

    // a vector
    constexpr auto a = 2.0 * mito::tensor::e_0<3> + mito::tensor::e_1<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::tensor::one_form(a, metric);

    // a vector
    constexpr auto xi = mito::tensor::e_2<3>;

    // evaluating {a_tilda} on {xi} yields 0.0
    static_assert(a_tilda(xi) == 0.0);
}