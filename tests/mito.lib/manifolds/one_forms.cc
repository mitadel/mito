// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>

TEST(Manifolds, OneForms)
{
    // the euclidean metric in 3D space
    constexpr auto metric = mito::identity<3>;

    // a vector field
    constexpr auto a = 2.0 * mito::e_0<3> + mito::e_1<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::manifolds::one_form(a, metric);

    // a vector
    constexpr auto xi = mito::e_2<3>;

    // evaluating {a_tilda} on {xi} yields 0.0
    static_assert(a_tilda(xi) == 0.0);
}