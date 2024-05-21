// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/tensor.h>


using mito::tensor::_;


TEST(Tensor, ContractionsTensor)
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

TEST(Tensor, ContractionsWedge)
{
    // the basis vectors
    constexpr auto e_x = mito::e_0<3>;
    constexpr auto e_y = mito::e_1<3>;
    constexpr auto e_z = mito::e_2<3>;

    // the corresponding dual basis
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);
    constexpr auto dz = mito::tensor::one_form(e_z);

    // wedge product of the three one-forms
    constexpr auto w = mito::tensor::wedge(dx, dy, dz);

    // contract the first index
    constexpr auto wx = w(e_x, _, _);

    // check result of double contraction
    static_assert(wx(e_y, e_z) == w(e_x, e_y, e_z));

    // contract the second index
    constexpr auto wy = w(_, e_y, _);

    // check result of double contraction
    static_assert(wy(e_x, e_z) == w(e_x, e_y, e_z));

    // contract the third index
    constexpr auto wz = w(_, _, e_z);

    // check result of double contraction
    static_assert(wz(e_x, e_y) == w(e_x, e_y, e_z));
}


// end of file
