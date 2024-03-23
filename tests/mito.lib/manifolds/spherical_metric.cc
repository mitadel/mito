// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// strip the namespace
using mito::geometry::SPHERICAL;
// alias for a set of spherical coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, SPHERICAL>;

// the basis for vector fields (e_r, e_theta, e_phi)
static constexpr auto e_r = mito::manifolds::uniform_field<3, SPHERICAL>(mito::e_0<3>);
static constexpr auto e_t = mito::manifolds::uniform_field<3, SPHERICAL>(mito::e_1<3>);
static constexpr auto e_p = mito::manifolds::uniform_field<3, SPHERICAL>(mito::e_2<3>);

// the basis for diagonal second-order tensor fields (e_rr, e_thetatheta, e_phiphi)
static constexpr auto e_rr = mito::manifolds::uniform_field<3, SPHERICAL>(mito::e_00<3>);
static constexpr auto e_tt = mito::manifolds::uniform_field<3, SPHERICAL>(mito::e_11<3>);
static constexpr auto e_pp = mito::manifolds::uniform_field<3, SPHERICAL>(mito::e_22<3>);

// TOFIX: this test can be made of {static_assert}s (like test polar metric) with c++26, when the
// math functions std::sin, std::cos and friends become {constexpr}

TEST(Manifolds, SphericalCoordinates)
{
    // the metric field
    constexpr auto g =
        // x[0] -> r
        // x[1] -> theta
        // x[2] -> phi
        mito::manifolds::field([](const coordinates_t & x) -> mito::diagonal_matrix_t<3> {
            // e_rr + r^2 * e_tt + r^2 sin^2(theta) * e_pp
            return e_rr(x) + (x[0] * x[0]) * e_tt(x)
                 + x[0] * std::sin(x[1]) * x[0] * std::sin(x[1]) * e_pp(x);
        });

    // the inverse metric field
    constexpr auto g_inv =
        // x[0] -> r
        // x[1] -> theta
        // x[2] -> phi
        mito::manifolds::field([](const coordinates_t & x) -> mito::diagonal_matrix_t<3> {
            // e_rr + 1/r^2 * e_tt + 1/(r^2 sin^2(theta)) * e_pp
            return e_rr(x) + 1.0 / (x[0] * x[0]) * e_tt(x)
                 + 1.0 / (x[0] * std::sin(x[1]) * x[0] * std::sin(x[1])) * e_pp(x);
        });

    // the basis one-forms
    constexpr auto dr = mito::manifolds::one_form(g_inv * e_r, g);
    constexpr auto dt = mito::manifolds::one_form(g_inv * e_t, g);
    constexpr auto dp = mito::manifolds::one_form(g_inv * e_p, g);

    // a point in space
    constexpr auto r = 2.0;
    constexpr auto t = std::numbers::pi;
    constexpr auto p = 0.25 * std::numbers::pi;
    constexpr auto x = mito::geometry::coordinates<SPHERICAL>({ r, t, p });

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    EXPECT_DOUBLE_EQ(1.0, dr(x)(e_r(x)));
    EXPECT_DOUBLE_EQ(0.0, dr(x)(e_t(x)));
    EXPECT_DOUBLE_EQ(0.0, dr(x)(e_p(x)));
    EXPECT_DOUBLE_EQ(0.0, dt(x)(e_r(x)));
    EXPECT_DOUBLE_EQ(1.0, dt(x)(e_t(x)));
    EXPECT_DOUBLE_EQ(0.0, dt(x)(e_p(x)));
    EXPECT_DOUBLE_EQ(0.0, dp(x)(e_r(x)));
    EXPECT_DOUBLE_EQ(0.0, dp(x)(e_t(x)));
    EXPECT_DOUBLE_EQ(1.0, dp(x)(e_p(x)));

    // the metric volume element
    constexpr auto w =
        mito::manifolds::sqrt(mito::manifolds::determinant(g)) * mito::manifolds::wedge(dr, dt, dp);

    constexpr auto dr_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dt_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dp_scalar = mito::scalar_t{ 0.01 };
    EXPECT_DOUBLE_EQ(
        r * r * std::sin(t) * dr_scalar * dt_scalar * dp_scalar,
        w(x)(dr_scalar * e_r(x), dt_scalar * e_t(x), dp_scalar * e_p(x)));
}


// end of file
