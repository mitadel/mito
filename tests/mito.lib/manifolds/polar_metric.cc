// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// the basis for vector fields (e_r and e_theta)
static constexpr auto e_r = mito::manifolds::uniform_field<2, mito::geometry::POLAR>(mito::e_0<2>);
static constexpr auto e_t = mito::manifolds::uniform_field<2, mito::geometry::POLAR>(mito::e_1<2>);

// the basis for diagonal second-order tensor fields (e_rr and e_thetatheta)
static constexpr auto e_rr =
    mito::manifolds::uniform_field<2, mito::geometry::POLAR>(mito::e_00<2>);
static constexpr auto e_tt =
    mito::manifolds::uniform_field<2, mito::geometry::POLAR>(mito::e_11<2>);

// alias for a set of polar coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;


TEST(Manifolds, PolarCoordinates)
{
    // the metric field
    constexpr auto g =
        // x[0] -> r
        // x[1] -> theta
        mito::manifolds::field([](const coordinates_t & x) -> mito::diagonal_matrix_t<2> {
            // e_rr + r^2 * e_tt
            return e_rr(x) + (x[0] * x[0]) * e_tt(x);
        });

    // the inverse metric field
    constexpr auto g_inv =
        // x[0] -> r
        // x[1] -> theta
        mito::manifolds::field([](const coordinates_t & x) -> mito::diagonal_matrix_t<2> {
            // e_r + 1/r^2 * e_theta
            return e_rr(x) + 1.0 / (x[0] * x[0]) * e_tt(x);
        });

    // the basis one-forms
    constexpr auto dr = mito::manifolds::one_form(g_inv * e_r, g);
    constexpr auto dt = mito::manifolds::one_form(g_inv * e_t, g);

    // a point in space
    constexpr auto r = 2.0;
    constexpr auto t = 0.0;
    constexpr auto x = mito::geometry::coordinates<mito::geometry::POLAR>({ r, t });

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dr(x)(e_r(x)) == 1.0);
    static_assert(dr(x)(e_t(x)) == 0.0);
    static_assert(dt(x)(e_r(x)) == 0.0);
    static_assert(dt(x)(e_t(x)) == 1.0);

    // the metric volume element
    constexpr auto w =
        mito::manifolds::sqrt(mito::manifolds::determinant(g)) * mito::manifolds::wedge(dr, dt);

    constexpr auto dr_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dt_scalar = mito::scalar_t{ 0.01 };
    static_assert(w(x)(dr_scalar * e_r(x), dt_scalar * e_t(x)) == r * dr_scalar * dt_scalar);
}


// end of file
