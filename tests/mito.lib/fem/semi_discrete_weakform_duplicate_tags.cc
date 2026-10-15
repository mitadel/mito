// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;

// concept to check if {termTs...} can form a semi-discrete weakform
template <typename... termTs>
concept can_form_weakform = requires { typename mito::fem::SemiDiscreteWeakform<termTs...>; };


TEST(Models, SEMI_DISCRETE_WEAKFORM_DUPLICATE_TAGS)
{
    // define the tags for the semi-discrete weakform
    struct Stiffness {};
    struct Inertia {};

    // the coefficient field
    auto coefficient = mito::functions::identity<coordinates_t, 2>();

    // the density field
    auto density = mito::functions::one<coordinates_t>;

    // the stiffness block type
    using stiffness_block_t = mito::fem::blocks::grad_grad_block_t<decltype(coefficient)>;

    // the mass block type
    using mass_block_t = mito::fem::blocks::value_value_block_t<decltype(density)>;

    // the stiffness term type
    using stiffness_term_t = mito::fem::term_t<Stiffness, stiffness_block_t>;

    // the second stiffness term type; same tag, different block type
    using stiffness_duplicate_term_t = mito::fem::term_t<Stiffness, mass_block_t>;

    // the mass term type
    using mass_term_t = mito::fem::term_t<Inertia, mass_block_t>;

    // the semi-discrete weakform
    static_assert(mito::fem::distinct_tags_c<stiffness_term_t, mass_term_t>);
    static_assert(can_form_weakform<stiffness_term_t, mass_term_t>);

    // duplicate tags are not allowed
    static_assert(!mito::fem::distinct_tags_c<stiffness_term_t, stiffness_term_t>);
    static_assert(!can_form_weakform<stiffness_term_t, stiffness_term_t>);

    // terms with the same tag are not allowed
    static_assert(!mito::fem::distinct_tags_c<stiffness_term_t, stiffness_duplicate_term_t>);
    static_assert(!can_form_weakform<stiffness_term_t, stiffness_duplicate_term_t>);

    // terms with different tags but the same block type are allowed
    static_assert(mito::fem::distinct_tags_c<stiffness_term_t, mass_term_t>);
    static_assert(can_form_weakform<mass_term_t, stiffness_duplicate_term_t>);

    // all done
    return;
}