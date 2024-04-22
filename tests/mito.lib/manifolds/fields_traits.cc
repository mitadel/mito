// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the function extracting the components of 2D vector
static constexpr auto x0 = mito::manifolds::coordinate<coordinates_t, 0>;
static constexpr auto x1 = mito::manifolds::coordinate<coordinates_t, 1>;


TEST(Manifolds, FieldsTraits)
{
    // cos(x0 * x1)
    constexpr auto cos = mito::functions::cos(x0 * x1);

    // a scalar field
    constexpr auto f = mito::manifolds::field(cos);

    // assert that {f} is a scalar field
    static_assert(mito::manifolds::scalar_field_c<decltype(f)>);

    // the function returning the constant e0 unit vector in 2D
    [[maybe_unused]] constexpr auto e0 =
        mito::manifolds::uniform_field<coordinates_t>(mito::e_0<2>);

    // the function returning the constant e1 unit vector in 2D
    [[maybe_unused]] constexpr auto e1 =
        mito::manifolds::uniform_field<coordinates_t>(mito::e_1<2>);

    // TOFIX: implement field * function and function * field
    // // a vector field
    // constexpr auto g = mito::manifolds::field(cos * (e0 + e1));
    // // assert that {g} is a vector field
    // static_assert(mito::manifolds::vector_field_c<decltype(g)>);

    // // a second-order tensor field (2x2 identity tensor field in 3 dimensional space)
    // constexpr auto i = mito::manifolds::identity_tensor_field<coordinates_t, 3>;
    // // assert that {i} is a tensor field
    // static_assert(mito::manifolds::tensor_field_c<decltype(i)>);
    // // assert that {i} is a symmetric tensor field
    // static_assert(mito::manifolds::symmetric_tensor_field_c<decltype(i)>);
    // // assert that {i} is a diagonal tensor field
    // static_assert(mito::manifolds::diagonal_tensor_field_c<decltype(i)>);
}


// end of file
