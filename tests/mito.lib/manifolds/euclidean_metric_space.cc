// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// use cartesian coordinates
using mito::geometry::CARTESIAN;

// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, CARTESIAN>;


TEST(Manifolds, EuclideanMetricSpace)
{
    constexpr auto space = mito::geometry::metric_space<coordinates_t>();

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ 1.0, 1.0 });

    // the basis vectors
    constexpr auto e_0 = space.e<0>();
    // check that it is equal to [1.0, 0.0] at point {x}
    static_assert(e_0(x) == mito::vector_t<2>({ 1.0, 0.0 }));

    constexpr auto e_1 = space.e<1>();
    // check that it is equal to [0.0, 1.0] at point {x}
    static_assert(e_1(x) == mito::vector_t<2>({ 0.0, 1.0 }));

    // the basis one-forms
    constexpr auto dx_0 = space.dx<0>();
    constexpr auto dx_1 = space.dx<1>();

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dx_0(x)(e_0(x)) == 1.0);
    static_assert(dx_0(x)(e_1(x)) == 0.0);
    static_assert(dx_1(x)(e_0(x)) == 0.0);
    static_assert(dx_1(x)(e_1(x)) == 1.0);

    // the metric volume element
    constexpr auto w = space.w();

    // check that the volume differential is correct
    constexpr auto dx0_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dx1_scalar = mito::scalar_t{ 0.01 };
    static_assert(w(x)(dx0_scalar * e_0(x), dx1_scalar * e_1(x)) == dx0_scalar * dx1_scalar);

    // TOFIX: support this as well
    // (dx1(e1))({ 0.0, 0.0 });
}


// end of file
