// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// alias for a set of cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;


TEST(Manifolds, CartesianGradient)
{
    // the basis vectors
    constexpr auto e_0 = metric_space_t::e<0>;
    constexpr auto e_1 = metric_space_t::e<1>;

    // the basis one-forms
    constexpr auto dx0 = metric_space_t::dx<0>;
    constexpr auto dx1 = metric_space_t::dx<1>;

    // the function extracting the components of 2D vector
    constexpr auto x0 = mito::functions::component<coordinates_t, 0>;
    constexpr auto x1 = mito::functions::component<coordinates_t, 1>;

    // a scalar field
    constexpr auto f = mito::fields::field(x0 * x1);

    // df/dx[0] = x1
    constexpr auto df0 = mito::fields::derivative<0>(f);

    // df/dx[1] = x0
    constexpr auto df1 = mito::fields::derivative<1>(f);

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ 1.0, 1.0 });

    // the gradient form
    constexpr auto gradient_form = df0 * dx0 + df1 * dx1;

    // check that the contraction of the gradient form with the basis vectors yields the partial
    // derivatives
    static_assert(gradient_form(x)(e_0(x)) == df0(x));
    static_assert(gradient_form(x)(e_1(x)) == df1(x));

    // the gradient vector
    constexpr auto grad_vector = metric_space_t::metric_equivalent(gradient_form);

    //  the well-known formula for the gradient vector in euclidean coordinates
    constexpr auto formula = df0 * e_0 + df1 * e_1;

    // check that the gradient vector can also be computed with the well-known formula
    static_assert(grad_vector(x) == formula(x));
}


// end of file
