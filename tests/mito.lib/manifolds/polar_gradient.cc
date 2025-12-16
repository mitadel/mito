// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/manifolds.h>


// alias for a set of polar coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;

// pi sixth
constexpr auto pi_sixth = std::numbers::pi / 6.0;


TEST(Manifolds, PolarGradient)
{
    // the basis vectors
    constexpr auto e_r = mito::geometry::polar::e_r;
    constexpr auto e_t = mito::geometry::polar::e_theta;

    // the basis one-forms
    constexpr auto dr = metric_space_t::dx<0>;
    constexpr auto dt = metric_space_t::dx<1>;

    // the function extracting the {r} component
    constexpr auto r = mito::geometry::polar::r;
    // the function extracting the {theta} component
    constexpr auto theta = mito::geometry::polar::theta;

    // a scalar field
    constexpr auto f = r * mito::functions::sin(theta);

    // df/dr = sin(theta)
    constexpr auto df0 = mito::functions::derivative<0>(f);

    // df/dtheta = r * cos(theta)
    constexpr auto df1 = mito::functions::derivative<1>(f);

    // a point in space {r = 2.0, theta = pi / 6.0}
    constexpr auto x = mito::geometry::polar::coordinates({ 2.0, pi_sixth });

    // the gradient form
    constexpr auto gradient_form = df0 * dr + df1 * dt;

    // check that the contraction of the gradient form with the basis vectors yields the partial
    // derivatives
    static_assert(gradient_form(x)(e_r(x)) == df0(x));
    static_assert(gradient_form(x)(e_t(x)) == df1(x));

    // the gradient vector
    constexpr auto grad_vector = metric_space_t::metric_equivalent(gradient_form);

    //  the well-known formula for the gradient vector in polar coordinates
    constexpr auto formula = df0 * e_r + (1.0 / (r * r)) * df1 * e_t;

    // check that the gradient vector can also be computed with the well-known formula
    static_assert(grad_vector(x) == formula(x));
}


// end of file
