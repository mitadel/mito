// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;

TEST(Models, SEMI_DISCRETE_WEAKFORM_GET)
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
    auto weakform = mito::fem::SemiDiscreteWeakform(
        mito::fem::term<Stiffness>(std::move(grad_grad_block)),
        mito::fem::term<Inertia>(std::move(mass_block)),
        mito::fem::term<Load>(std::move(load_block)));

    // check that each term can be retrieved by its tag
    static_assert(std::is_same_v<decltype(weakform.get<Stiffness>()), decltype(grad_grad_block) &>);
    static_assert(std::is_same_v<decltype(weakform.get<Inertia>()), decltype(mass_block) &>);
    static_assert(std::is_same_v<decltype(weakform.get<Load>()), decltype(load_block) &>);

    // check that each term can be retrieved by its tag from a const weakform
    const auto & cweakform = weakform;
    static_assert(
        std::is_same_v<decltype(cweakform.get<Stiffness>()), const decltype(grad_grad_block) &>);
    static_assert(std::is_same_v<decltype(cweakform.get<Inertia>()), const decltype(mass_block) &>);
    static_assert(std::is_same_v<decltype(cweakform.get<Load>()), const decltype(load_block) &>);

    // all done
    return;
}