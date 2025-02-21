// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fields.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the function extracting the components of 2D vector
static constexpr auto x0 = mito::functions::component<coordinates_t, 0>;
static constexpr auto x1 = mito::functions::component<coordinates_t, 1>;


TEST(Fields, Traits)
{
    // cos(x0 * x1)
    constexpr auto cos = mito::functions::cos(x0 * x1);

    // a scalar field
    constexpr auto f = mito::fields::field(cos);

    // assert that {f} is a scalar field
    static_assert(mito::fields::scalar_field_c<decltype(f)>);

    // the function returning the constant e0 unit vector in 2D
    [[maybe_unused]] constexpr auto e0 =
        mito::fields::uniform_field<coordinates_t>(mito::tensor::e_0<2>);

    // the function returning the constant e1 unit vector in 2D
    [[maybe_unused]] constexpr auto e1 =
        mito::fields::uniform_field<coordinates_t>(mito::tensor::e_1<2>);

    // a vector field
    constexpr auto g = mito::fields::field(cos * (e0 + e1));
    // assert that {g} is a vector field
    static_assert(mito::fields::vector_field_c<decltype(g)>);

    // a second-order tensor field (2x2 identity tensor field in 3 dimensional space)
    constexpr auto i = mito::fields::identity_tensor_field<coordinates_t, 3>;
    // assert that {i} is a tensor field
    static_assert(mito::fields::tensor_field_c<decltype(i)>);
    // assert that {i} is a symmetric tensor field
    static_assert(mito::fields::symmetric_tensor_field_c<decltype(i)>);
    // assert that {i} is a diagonal tensor field
    static_assert(mito::fields::diagonal_tensor_field_c<decltype(i)>);
}


// end of file
