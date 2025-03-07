// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/tensor.h>


TEST(Tensor, FormsTensorProduct)
{
    // the Euclidean metric in 3D space
    constexpr auto metric = mito::tensor::identity<3>;

    // a vector
    constexpr auto a = mito::tensor::e_0<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::tensor::one_form(a, metric);

    // another vector
    constexpr auto b = mito::tensor::e_1<3>;

    // the corresponding one-form
    constexpr auto b_tilda = mito::tensor::one_form(b, metric);

    // tensor product of two one-forms
    constexpr auto a_tensor_b = mito::tensor::tensor(a_tilda, b_tilda);

    // a vector
    constexpr auto xi0 = mito::tensor::e_0<3>;

    // another vector
    constexpr auto xi1 = mito::tensor::e_1<3>;

    // check result of double contraction
    static_assert(a_tensor_b(xi0, xi1) == 1.0);
    static_assert(a_tensor_b(xi1, xi0) == 0.0);

    // another vector
    constexpr auto c = mito::tensor::e_2<3>;

    // the corresponding one-form
    constexpr auto c_tilda = mito::tensor::one_form(c, metric);

    // triple tensor product
    constexpr auto a_tensor_b_tensor_c = mito::tensor::tensor(a_tilda, b_tilda, c_tilda);

    // another vector
    constexpr auto xi2 = mito::tensor::e_2<3>;

    // check result of triple contraction
    static_assert(a_tensor_b_tensor_c(xi0, xi1, xi2) == 1.0);
    static_assert(a_tensor_b_tensor_c(xi2, xi0, xi1) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi1, xi2, xi0) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi0, xi2, xi1) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi2, xi1, xi0) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi1, xi0, xi2) == 0.0);
}


// end of file
