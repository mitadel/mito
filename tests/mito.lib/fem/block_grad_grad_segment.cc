// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;
// the type of coordinate system
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::segment_t<1>;
// the reference simplex
using reference_simplex_t = mito::geometry::reference_segment_t;
// Gauss quadrature on segments with degree of exactness 2
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_simplex_t, 2>;

// instantiate the quadrature rule
constexpr auto quadrature_rule = quadrature_rule_t();


TEST(Fem, BlockGradGradSegment)
{
    // the coordinate system
    auto coord_system = coord_system_t();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::segment<1>({ node_0, node_1 });

    {
        // first order isoparametric segment
        using element_p1_t = mito::fem::isoparametric_simplex_t<1, cell_t>;

        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();

        // a finite element
        auto element_p1 = element_p1_t(
            geometric_simplex, coord_system, { discretization_node_0, discretization_node_1 });

        // a grad-grad matrix block
        auto grad_grad_block =
            mito::fem::blocks::grad_grad_block<element_p1_t, quadrature_rule_t>();

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