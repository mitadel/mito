// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// alias for a set of cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Manifolds, VectorFields)
{
    // a point in space
    constexpr auto x = mito::geometry::coordinates({ 0.0, 0.0 });

    // a scalar field
    constexpr auto f = mito::manifolds::field(
        [](const coordinates_t & x) -> mito::scalar_t { return std::cos(x[0] * x[1]); });

    // check value of field at {x}
    static_assert(f(x) == 1.0);

    // a vector field
    constexpr auto g = mito::manifolds::field([](const coordinates_t & x) -> mito::vector_t<2> {
        return { std::cos(x[0] * x[1]), std::cos(x[0] * x[1]) };
    });

    // check value of field at {x}
    static_assert(g(x) == mito::vector_t<2>{ 1.0, 1.0 });
}


// end of file
