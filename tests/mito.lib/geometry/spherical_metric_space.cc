// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// use spherical coordinates
using mito::geometry::SPHERICAL;

// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<3, SPHERICAL>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;


TEST(Manifolds, SphericalMetricSpace)
{
    // the basis vectors
    constexpr auto e_r = metric_space_t::e<0>;
    constexpr auto e_t = metric_space_t::e<1>;
    constexpr auto e_p = metric_space_t::e<2>;

    // the basis one-forms
    constexpr auto dr = metric_space_t::dx<0>;
    constexpr auto dt = metric_space_t::dx<1>;
    constexpr auto dp = metric_space_t::dx<2>;

    // a point in space
    constexpr auto r = 2.0;
    constexpr auto t = std::numbers::pi;
    constexpr auto p = 0.25 * std::numbers::pi;
    constexpr auto x = mito::geometry::spherical::coordinates({ r, t, p });

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dr(x)(e_r(x)) == 1.0);
    static_assert(dr(x)(e_t(x)) == 0.0);
    static_assert(dr(x)(e_p(x)) == 0.0);
    static_assert(dt(x)(e_r(x)) == 0.0);
    static_assert(dt(x)(e_t(x)) == 1.0);
    static_assert(dt(x)(e_p(x)) == 0.0);
    static_assert(dp(x)(e_r(x)) == 0.0);
    static_assert(dp(x)(e_t(x)) == 0.0);
    static_assert(dp(x)(e_p(x)) == 1.0);

    // the metric volume element
    constexpr auto w = metric_space_t::w;

    // check that the volume differential is correct
    constexpr auto dr_scalar = mito::tensor::scalar_t{ 0.01 };
    constexpr auto dt_scalar = mito::tensor::scalar_t{ 0.01 };
    constexpr auto dp_scalar = mito::tensor::scalar_t{ 0.01 };
    EXPECT_DOUBLE_EQ(
        r * r * std::sin(t) * dr_scalar * dt_scalar * dp_scalar,
        w(x)(dr_scalar * e_r(x), dt_scalar * e_t(x), dp_scalar * e_p(x)));
}


// end of file
