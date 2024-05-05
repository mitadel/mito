// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// alias for a set of cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the basis for vector fields
static constexpr auto e_0 = mito::fields::uniform_field<coordinates_t>(mito::e_0<2>);
static constexpr auto e_1 = mito::fields::uniform_field<coordinates_t>(mito::e_1<2>);

// the function extracting the components of 2D vector
static constexpr auto x0 = mito::functions::component<coordinates_t, 0>;
static constexpr auto x1 = mito::functions::component<coordinates_t, 1>;


TEST(Manifolds, CartesianGradient)
{
    // a scalar field
    constexpr auto f = mito::fields::field(x0 * x1);

    // df/dx[0]
    constexpr auto df0 = mito::fields::derivative<0>(f);

    // df/dx[1]
    constexpr auto df1 = mito::fields::derivative<1>(f);

    // the Euclidean metric
    constexpr auto g = mito::manifolds::metric<coordinates_t>::field();

    // the gradient form
    constexpr auto gradient = mito::fields::one_form_field(df0 * e_0 + df1 * e_1, g);

    // a point in space
    constexpr auto x = mito::geometry::coordinates<coordinates_t>({ 1.0, 1.0 });

    // check that the contractions of the gradient form with the basis vectors return the partial
    // derivatives of {f}
    static_assert(gradient(x)(e_0(x)) == df0(x));
    static_assert(gradient(x)(e_1(x)) == df1(x));
}


// end of file
