// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// alias for a set of spherical coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::SPHERICAL>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;

// pi sixth
constexpr auto pi_sixth = std::numbers::pi / 6.0;
constexpr auto pi_fourth = std::numbers::pi / 4.0;


TEST(Manifolds, SphericalGradient)
{
    // the basis vectors
    constexpr auto e_r = metric_space_t::e<0>;
    constexpr auto e_t = metric_space_t::e<1>;
    constexpr auto e_p = metric_space_t::e<2>;

    // the basis one-forms
    constexpr auto dr = metric_space_t::dx<0>;
    constexpr auto dt = metric_space_t::dx<1>;
    constexpr auto dp = metric_space_t::dx<2>;

    // the function extracting the 0th component (r)
    constexpr auto r = mito::functions::component<coordinates_t, 0>;
    // the function extracting the 1st component (theta)
    constexpr auto theta = mito::functions::component<coordinates_t, 1>;
    // the function extracting the 2nd component (phi)
    constexpr auto phi = mito::functions::component<coordinates_t, 2>;

    // a scalar field
    constexpr auto f =
        mito::fields::field(r * mito::functions::sin(theta) * mito::functions::cos(phi));

    // df/dr = sin(theta) * cos(phi)
    constexpr auto df0 = mito::fields::derivative<0>(f);

    // df/dtheta = r * cos(theta) * cos(phi)
    constexpr auto df1 = mito::fields::derivative<1>(f);

    // df/dphi = - r * cos(theta) * sin(phi)
    constexpr auto df2 = mito::fields::derivative<2>(f);

    // a point in space {r = 2.0, theta = pi / 6.0, phi = pi / 6.0}
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ 2.0, pi_sixth, pi_fourth });

    // the gradient form
    constexpr auto gradient_form = df0 * dr + df1 * dt + df2 * dp;

    // check that the contraction of the gradient form with the basis vectors yields the partial
    // derivatives
    EXPECT_DOUBLE_EQ(df0(x), gradient_form(x)(e_r(x)));
    EXPECT_DOUBLE_EQ(df1(x), gradient_form(x)(e_t(x)));
    EXPECT_DOUBLE_EQ(df2(x), gradient_form(x)(e_p(x)));

    // the gradient vector
    constexpr auto grad_vector = metric_space_t::metric_equivalent(gradient_form);

    //  the well-known formula for the gradient vector in spherical coordinates
    constexpr auto formula =
        df0 * e_r + (1.0 / (r * r)) * df1 * e_t
        + (1.0 / (r * r * mito::functions::sin(theta) * mito::functions::sin(theta))) * df2 * e_p;

    // check that the gradient vector can also be computed with the well-known formula
    EXPECT_TRUE(pyre::tensor::norm(grad_vector(x) - formula(x)) < 1.e-15);
}


// end of file
