// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/fields.h>


// use polar coordinates
using mito::geometry::POLAR;

// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, POLAR>;

// the function extracting the x_0 (i.e. the radial) component of 2D vector
static constexpr auto r = mito::functions::component<coordinates_t, 0>;

// the basis for vector fields (e_r and e_theta)
static constexpr auto e_r = mito::fields::uniform_field<coordinates_t>(mito::e_0<2>);
static constexpr auto e_t = mito::fields::uniform_field<coordinates_t>(mito::e_1<2>);

// the basis for diagonal second-order tensor fields (e_rr and e_thetatheta)
static constexpr auto e_rr = mito::fields::uniform_field<coordinates_t>(mito::e_00<2>);
static constexpr auto e_tt = mito::fields::uniform_field<coordinates_t>(mito::e_11<2>);


TEST(Manifolds, PolarCoordinates)
{
    // the metric field
    constexpr auto g = e_rr + r * r * e_tt;

    // the inverse metric field
    constexpr auto g_inv = e_rr + 1.0 / (r * r) * e_tt;

    // the basis one-forms
    constexpr auto dr = mito::fields::one_form_field(g_inv * e_r, g);
    constexpr auto dt = mito::fields::one_form_field(g_inv * e_t, g);

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
    constexpr auto w =
        mito::fields::sqrt(mito::fields::determinant(g)) * mito::fields::wedge(dr, dt);

    constexpr auto dr_scalar = mito::scalar_t{ 0.01 };
    constexpr auto dt_scalar = mito::scalar_t{ 0.01 };
    static_assert(w(x)(dr_scalar * e_r(x), dt_scalar * e_t(x)) == r * dr_scalar * dt_scalar);
}


// end of file
