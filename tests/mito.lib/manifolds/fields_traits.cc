// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// use CARTESIAN coordinates
using mito::geometry::CARTESIAN;


TEST(Manifolds, FieldsTraits)
{
    // cos(x0 * x1)
    constexpr auto cos = mito::functions::cos(mito::functions::x<0, 2> * mito::functions::x<1, 2>);

    // a scalar field
    constexpr auto f = mito::manifolds::field<CARTESIAN>(cos);

    // assert that {f} is a scalar field
    static_assert(mito::manifolds::ScalarField<decltype(f)>);

    // the function returning the constant e0 unit vector in 2D
    constexpr auto e0 = mito::functions::constant<mito::vector_t<2>>(mito::e_0<2>);

    // the function returning the constant e1 unit vector in 2D
    constexpr auto e1 = mito::functions::constant<mito::vector_t<2>>(mito::e_1<2>);

    // a vector field
    constexpr auto g = mito::manifolds::field<CARTESIAN>(cos * (e0 + e1));
    // assert that {g} is a vector field
    static_assert(mito::manifolds::VectorField<decltype(g)>);

    // a second-order tensor field (2x2 identity tensor field in 3 dimensional space)
    constexpr auto i = mito::manifolds::identity_tensor_field<2, 3>;
    // assert that {i} is a tensor field
    static_assert(mito::manifolds::TensorField<decltype(i)>);
    // assert that {i} is a symmetric tensor field
    static_assert(mito::manifolds::SymmetricTensorField<decltype(i)>);
    // assert that {i} is a diagonal tensor field
    static_assert(mito::manifolds::DiagonalTensorField<decltype(i)>);
}


// end of file
