// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;

TEST(Models, SEMI_DISCRETE_WEAKFORM_COMPUTE_BLOCK)
{
    // define the tags for the semi-discrete weakform
    struct Stiffness {};

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system_t<coordinates_t>();

    // an atlas under the coordinate system
    auto atlas = mito::manifolds::atlas<cell_t>(coord_system);

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // make a geometric simplex
    auto triangle = mito::geometry::triangle(node_0, node_1, node_2);

    // make a manifold element from the segment
    auto element = mito::manifolds::parametrized_element(
        triangle, atlas.parametrization(triangle), metric_space_t::w);

    // build the discretization nodes
    auto discretization_node_0 = discretization_node_t();
    auto discretization_node_1 = discretization_node_t();
    auto discretization_node_2 = discretization_node_t();
    auto discretization_node_3 = discretization_node_t();
    auto discretization_node_4 = discretization_node_t();
    auto discretization_node_5 = discretization_node_t();

    // the degree of the finite element
    constexpr int degree = 2;
    
    // assemble the finite element type
    using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

    // a finite element
    auto element_p2 = mito::fem::finite_element<finite_element_t>(
        element, { discretization_node_0, discretization_node_1, discretization_node_2,
                   discretization_node_3, discretization_node_4, discretization_node_5 });

    // the coefficient field
    auto coefficient = mito::functions::identity<coordinates_t, 2>();

    // a grad grad matrix block
    auto grad_grad_block = mito::fem::blocks::grad_grad_block(coefficient);

    // the semi-discrete weakform
    auto weakform = mito::fem::SemiDiscreteWeakform(
      mito::fem::term<Stiffness>(std::move(grad_grad_block)));

    // get the reference grad grad block for comparison
    auto reference_grad_grad = mito::fem::blocks::grad_grad_block(coefficient);

    // compute the expected block using the reference grad grad block
    auto expected_matrix = reference_grad_grad.compute(element_p2);

    // `element` is built the same way as in your other FEM tests
    auto weakform_matrix = reference_grad_grad.compute(element_p2);

    // the semi-discrete weakform's own dispatch should agree too
    EXPECT_NEAR(mito::tensor::norm(expected_matrix - weakform_matrix), 0.0, 1e-14);

    // all done
    return;
}