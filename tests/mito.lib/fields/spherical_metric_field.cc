// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fields.h>


// strip the namespace
using mito::geometry::SPHERICAL;
// alias for a set of spherical coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, SPHERICAL>;

// the function extracting the x_0 (i.e. the radial) component of a 3D vector
static constexpr auto r = mito::functions::component<coordinates_t, 0>;
// the function extracting the x_1 (i.e. the theta) component of a 3D vector
static constexpr auto t = mito::functions::component<coordinates_t, 1>;


TEST(Manifolds, SphericalCoordinates)
{
    // the basis for vector fields (e_r, e_theta, e_phi)
    constexpr auto e_r = mito::fields::uniform_field<coordinates_t>(mito::tensor::e_0<3>);
    constexpr auto e_t = r * mito::fields::uniform_field<coordinates_t>(mito::tensor::e_1<3>);
    constexpr auto e_p = r * mito::functions::sin(t)
                       * mito::fields::uniform_field<coordinates_t>(mito::tensor::e_2<3>);

    // the basis for diagonal second-order tensor fields (e_rr, e_thetatheta, e_phiphi)
    constexpr auto e_rr = mito::fields::uniform_field<coordinates_t>(mito::tensor::e_00<3>);
    constexpr auto e_tt = mito::fields::uniform_field<coordinates_t>(mito::tensor::e_11<3>);
    constexpr auto e_pp = mito::fields::uniform_field<coordinates_t>(mito::tensor::e_22<3>);

    // the metric field
    constexpr auto g = (e_r * e_r) * e_rr + (e_t * e_t) * e_tt + (e_p * e_p) * e_pp;

    // the inverse metric field
    constexpr auto g_inv = mito::fields::inverse(g);

    // the basis one-forms
    constexpr auto dr = mito::fields::field(
        [e_r, g_inv](const coordinates_t & x) { return mito::tensor::one_form(e_r(x), g_inv(x)); });
    constexpr auto dt = mito::fields::field(
        [e_t, g_inv](const coordinates_t & x) { return mito::tensor::one_form(e_t(x), g_inv(x)); });
    constexpr auto dp = mito::fields::field(
        [e_p, g_inv](const coordinates_t & x) { return mito::tensor::one_form(e_p(x), g_inv(x)); });

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

    constexpr auto dr_scalar = mito::tensor::scalar_t{ 0.01 };
    constexpr auto dt_scalar = mito::tensor::scalar_t{ 0.01 };
    constexpr auto dp_scalar = mito::tensor::scalar_t{ 0.01 };
    EXPECT_DOUBLE_EQ(
        r * r * std::sin(t) * dr_scalar * dt_scalar * dp_scalar,
        w(x)(dr_scalar * e_r(x), dt_scalar * e_t(x), dp_scalar * e_p(x)));
}


// end of file
