// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// use cartesian coordinates
using mito::geometry::CARTESIAN;

// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, CARTESIAN>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;


TEST(Manifolds, EuclideanMetricSpace)
{
    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ 1.0, 1.0 });

    // the basis vectors
    constexpr auto e_0 = metric_space_t::e<0>;
    // check that it is equal to [1.0, 0.0] at point {x}
    static_assert(e_0(x) == mito::vector_t<2>({ 1.0, 0.0 }));

    constexpr auto e_1 = metric_space_t::e<1>;
    // check that it is equal to [0.0, 1.0] at point {x}
    static_assert(e_1(x) == mito::vector_t<2>({ 0.0, 1.0 }));

    // the basis one-forms
    constexpr auto dx_0 = metric_space_t::dx<0>;
    constexpr auto dx_1 = metric_space_t::dx<1>;

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dx_0(x)(e_0(x)) == 1.0);
    static_assert(dx_0(x)(e_1(x)) == 0.0);
    static_assert(dx_1(x)(e_0(x)) == 0.0);
    static_assert(dx_1(x)(e_1(x)) == 1.0);

    // the metric volume element
    constexpr auto w = metric_space_t::w;

    // check that the volume differential is correct
    constexpr auto dx0_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dx1_scalar = mito::scalar_t{ 0.01 };
    static_assert(w(x)(dx0_scalar * e_0(x), dx1_scalar * e_1(x)) == dx0_scalar * dx1_scalar);
}


// end of file
