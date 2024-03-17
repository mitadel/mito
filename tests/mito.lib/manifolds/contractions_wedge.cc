// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


using mito::manifolds::_;


TEST(Manifolds, ContractionsWedge)
{
    // the basis vectors
    constexpr auto e_x = mito::e_0<3>;
    constexpr auto e_y = mito::e_1<3>;
    constexpr auto e_z = mito::e_2<3>;

    // the corresponding dual basis
    constexpr auto dx = mito::manifolds::one_form(e_x);
    constexpr auto dy = mito::manifolds::one_form(e_y);
    constexpr auto dz = mito::manifolds::one_form(e_z);

    // wedge product of the three one-forms
    constexpr auto w = mito::manifolds::wedge(dx, dy, dz);

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
