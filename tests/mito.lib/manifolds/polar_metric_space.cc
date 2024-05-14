// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// use polar coordinates
using mito::geometry::POLAR;

// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, POLAR>;


TEST(Manifolds, PolarMetricSpace)
{
    constexpr auto space = mito::geometry::metric_space<coordinates_t>();

    // the basis vectors
    constexpr auto e_r = space.e<0>();
    constexpr auto e_t = space.e<1>();

    // the basis one-forms
    constexpr auto dr = space.dx<0>();
    constexpr auto dt = space.dx<1>();

    // a point in space
    constexpr auto r = 2.0;
    constexpr auto t = 0.0;
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ r, t });

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dr(x)(e_r(x)) == 1.0);
    static_assert(dr(x)(e_t(x)) == 0.0);
    static_assert(dt(x)(e_r(x)) == 0.0);
    static_assert(dt(x)(e_t(x)) == 1.0);

    // the metric volume element
    constexpr auto w = space.w();

    // check that the volume differential is correct
    constexpr auto dr_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dt_scalar = mito::scalar_t{ 0.01 };
    static_assert(w(x)(dr_scalar * e_r(x), dt_scalar * e_t(x)) == r * dr_scalar * dt_scalar);
}


// end of file
