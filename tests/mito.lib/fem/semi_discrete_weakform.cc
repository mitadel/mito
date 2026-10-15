// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// the material type
using material_t = mito::materials::thermal::ConstantIsotropic<2>;
// the model type
using model_t = mito::models::HeatDiffusion<material_t>;
// the term type
template <typename tagT, typename blockT>
using term_t = mito::fem::term_t<tagT, blockT>;


TEST(Models, SEMI_DISCRETE_WEAKFORM)
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
    // all done
    return;
}