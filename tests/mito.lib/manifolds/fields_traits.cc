// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// alias for a set of cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Manifolds, FieldsTraits)
{
    // a scalar field
    constexpr auto f = mito::manifolds::field(
        [](const coordinates_t & x) -> mito::scalar_t { return std::cos(x[0] * x[1]); });
    // assert that {f} is a scalar field
    static_assert(mito::manifolds::ScalarField<decltype(f)>);

    // a vector field
    constexpr auto g = mito::manifolds::field([](const coordinates_t & x) -> mito::vector_t<2> {
        return { std::cos(x[0] * x[1]), std::cos(x[0] * x[1]) };
    });
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
