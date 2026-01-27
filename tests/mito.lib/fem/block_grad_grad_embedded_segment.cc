// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates (2D physical space for embedded segments)
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of coordinate system
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell (segment embedded in 2D)
using cell_t = mito::geometry::segment_t<2>;
// the reference simplex
using reference_simplex_t = mito::geometry::reference_segment_t;
// Gauss quadrature on segments with degree of exactness 2
using quadrature_rule_t =
    mito::quadrature::quadrature_rule_t<mito::quadrature::GAUSS, reference_simplex_t, 2>;

// instantiate the quadrature rule
constexpr auto quadrature_rule = quadrature_rule_t();


TEST(Fem, BlockGradGradEmbeddedSegment)
{
    // the coordinate system
    auto coord_system = coord_system_t();

    // build nodes (unit-length diagonal segment embedded in 2D)
    constexpr auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { inv_sqrt2, inv_sqrt2 });

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::segment<2>({ node_0, node_1 });

    // create a mesh with a single segment
    auto mesh = mito::mesh::mesh<cell_t>();
    mesh.insert({ node_0, node_1 });

    // create normal field for the submanifold (perpendicular to diagonal)
    auto normal_field = mito::functions::constant<coordinates_t>(
        mito::tensor::vector_t<2>{ -inv_sqrt2, inv_sqrt2 });

    // create the submanifold
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, normal_field);

    {
        // first order isoparametric embedded segment
        using element_p1_t = mito::fem::isoparametric_simplex_t<1, decltype(manifold)>;

        // build the discretization nodes
        auto discretization_node_0 = discretization_node_t();
        auto discretization_node_1 = discretization_node_t();

        // a finite element
        auto element_p1 = element_p1_t(
            geometric_simplex, coord_system, { discretization_node_0, discretization_node_1 },
            manifold.volume_form());

        // a grad-grad matrix block
        auto grad_grad_block =
            mito::fem::blocks::grad_grad_block<element_p1_t, quadrature_rule_t>();

        // the analytical elementary stiffness matrix (same as 1D for unit-length segment)
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