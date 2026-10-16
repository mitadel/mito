// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;

TEST(Models, SEMI_DISCRETE_WEAKFORM_DIRECT_CONSTRUCTION)
{
    // define the tags for the semi-discrete weakform
    struct Stiffness {};
    struct Inertia {};
    struct Load {};

    // the coefficient field
    auto coefficient = mito::functions::identity<coordinates_t, 2>();

    // the density field
    auto density = mito::functions::one<coordinates_t>;

    // the load field
    auto load = mito::functions::one<coordinates_t>;

    // a grad grad matrix block
    auto grad_grad_block = mito::fem::blocks::grad_grad_block(coefficient);

    // a mass matrix block
    auto mass_block = mito::fem::blocks::value_value_block(density);

    // a load vector block
    auto load_block = mito::fem::blocks::value_block(load);

    // the semi-discrete weakform
    auto weakform = mito::fem::semi_discrete_weakform_t(
        mito::fem::term<Stiffness>(std::move(grad_grad_block)),
        mito::fem::term<Inertia>(std::move(mass_block)),
        mito::fem::term<Load>(std::move(load_block)));
    // all done
    return;
}