// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/fields.h>


// strip the namespace
using mito::geometry::SPHERICAL;
// alias for a set of spherical coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, SPHERICAL>;

// the function extracting the x_0 (i.e. the radial) component of a 3D vector
static constexpr auto r = mito::functions::component<coordinates_t, 0>;
// the function extracting the x_1 (i.e. the theta) component of a 3D vector
static constexpr auto t = mito::functions::component<coordinates_t, 1>;

// the basis for vector fields (e_r, e_theta, e_phi)
static constexpr auto e_r = mito::fields::uniform_field<coordinates_t>(mito::e_0<3>);
static constexpr auto e_t = mito::fields::uniform_field<coordinates_t>(mito::e_1<3>);
static constexpr auto e_p = mito::fields::uniform_field<coordinates_t>(mito::e_2<3>);

// the basis for diagonal second-order tensor fields (e_rr, e_thetatheta, e_phiphi)
static constexpr auto e_rr = mito::fields::uniform_field<coordinates_t>(mito::e_00<3>);
static constexpr auto e_tt = mito::fields::uniform_field<coordinates_t>(mito::e_11<3>);
static constexpr auto e_pp = mito::fields::uniform_field<coordinates_t>(mito::e_22<3>);


TEST(Manifolds, SphericalCoordinates)
{
    // the metric field
    // e_rr + r^2 * e_tt + r^2 sin^2(theta) * e_pp
    constexpr auto g =
        e_rr + r * r * e_tt + r * mito::functions::sin(t) * r * mito::functions::sin(t) * e_pp;

    // the inverse metric field
    // e_rr + 1/r^2 * e_tt + 1/(r^2 sin^2(theta)) * e_pp
    constexpr auto g_inv = e_rr + 1.0 / (r * r) * e_tt
                         + 1.0 / (r * mito::functions::sin(t) * r * mito::functions::sin(t)) * e_pp;

    // the basis one-forms
    constexpr auto dr = mito::fields::one_form_field(g_inv * e_r, g);
    constexpr auto dt = mito::fields::one_form_field(g_inv * e_t, g);
    constexpr auto dp = mito::fields::one_form_field(g_inv * e_p, g);

    // a point in space
    constexpr auto r = 2.0;
    constexpr auto t = std::numbers::pi;
    constexpr auto p = 0.25 * std::numbers::pi;
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ r, t, p });

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(1.0 == dr(x)(e_r(x)));
    static_assert(0.0 == dr(x)(e_t(x)));
    static_assert(0.0 == dr(x)(e_p(x)));
    static_assert(0.0 == dt(x)(e_r(x)));
    static_assert(1.0 == dt(x)(e_t(x)));
    static_assert(0.0 == dt(x)(e_p(x)));
    static_assert(0.0 == dp(x)(e_r(x)));
    static_assert(0.0 == dp(x)(e_t(x)));
    static_assert(1.0 == dp(x)(e_p(x)));

    // the metric volume element
    constexpr auto w =
        mito::fields::sqrt(mito::fields::determinant(g)) * mito::fields::wedge(dr, dt, dp);

    constexpr auto dr_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dt_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dp_scalar = mito::scalar_t{ 0.01 };
    EXPECT_DOUBLE_EQ(
        r * r * std::sin(t) * dr_scalar * dt_scalar * dp_scalar,
        w(x)(dr_scalar * e_r(x), dt_scalar * e_t(x), dp_scalar * e_p(x)));
}


// end of file
