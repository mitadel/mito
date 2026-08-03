// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;
// the metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::segment_t<1>;


TEST(Fem, BlockGradGradSegment)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system_t<coordinates_t>();

    // an atlas under the coordinate system
    auto atlas = mito::manifolds::atlas<cell_t>(coord_system);

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });

    // make a geometric simplex
    auto segment = mito::geometry::segment(node_0, node_1);

    // make a manifold element from the segment
    auto element = mito::manifolds::parametrized_element(
        segment, atlas.parametrization(segment), metric_space_t::w);

    {
        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();
        // the degree of the finite element
        constexpr int degree = 1;
        // assemble the finite element type
        using finite_element_t = mito::fem::finite_element_family<cell_t, degree>;

        // a finite element
        auto element_p1 = mito::fem::finite_element<finite_element_t>(
            element, { discretization_node_0, discretization_node_1 });

        // a grad-grad matrix block
        auto grad_grad_block = mito::fem::blocks::grad_grad_block<finite_element_t>();

        // the analytical elementary stiffness matrix
        auto analytical_block = mito::tensor::matrix_t<2>{ 1.0, -1.0, -1.0, 1.0 };

        // compute the elementary contribution of the block
        auto computed_block = grad_grad_block.compute(element_p1);

        // compute the error
        auto error = mito::tensor::norm(computed_block - analytical_block);

        // check the error is zero to machine precision
        EXPECT_DOUBLE_EQ(0.0, error);
    }

    // all done
    return;
}