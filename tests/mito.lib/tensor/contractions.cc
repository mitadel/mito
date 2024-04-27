// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/tensor.h>


using mito::tensor::_;


TEST(Manifolds, Contractions)
{
    // a vector
    constexpr auto a = mito::e_0<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::tensor::one_form(a);

    // another vector
    constexpr auto b = mito::e_1<3>;

    // the corresponding one-form
    constexpr auto b_tilda = mito::tensor::one_form(b);

    // tensor product of two one-forms
    constexpr auto a_tensor_b = mito::tensor::tensor(a_tilda, b_tilda);

    // a vector
    constexpr auto xi0 = mito::e_0<3>;

    // another vector
    constexpr auto xi1 = mito::e_1<3>;

    // check result of double contraction
    static_assert(a_tensor_b(xi0, xi1) == a_tilda(xi0) * b_tilda(xi1));

    // contract first index
    constexpr auto contraction1 = a_tensor_b(xi0, _);
    // check result
    static_assert(contraction1(xi1) == a_tensor_b(xi0, xi1));

    // contract second index
    constexpr auto contraction2 = a_tensor_b(_, xi1);
    // check result
    static_assert(contraction2(xi0) == a_tensor_b(xi0, xi1));
}


// end of file
