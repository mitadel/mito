// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// verify the butterfly interface element and the blocks computed on its trace: the jump-jump and
// average-average gradient blocks, unscaled by any physical coefficient

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates (2D physical space)
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the type of discretization node
using discretization_node_t = mito::discrete::discretization_node_t;
// the type of cell (segment embedded in 2D)
using cell_t = mito::geometry::segment_t<2>;
// the type of mesh node
using node_t = mito::geometry::node_t<2>;
// first degree finite elements on the interface
using interface_element_t = mito::fem::finite_element_family<cell_t, 1>;
// the traits of the butterfly element built from them
using interface_traits_t = mito::fem::InterfaceElementTraits<interface_element_t>;
// the type of a map between mesh nodes and discretization nodes
using node_map_t =
    std::unordered_map<node_t, discretization_node_t, mito::utilities::hash_function<node_t>>;

// degree of exactness 2 for both interface blocks
constexpr int doe = 2;


TEST(Fem, InterfaceElement)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system_t<coordinates_t>();

    // build nodes (unit-length diagonal segment embedded in 2D)
    constexpr auto inv_sqrt2 = 1.0 / std::sqrt(2.0);
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { inv_sqrt2, inv_sqrt2 });

    // the interface mesh, one segment running from {node_0} to {node_1}
    auto interface_mesh = mito::mesh::mesh<cell_t>();
    interface_mesh.insert({ node_0, node_1 });

    // the normal is the tangent rotated clockwise, so that it points from the left side of the
    // interface to its right side
    auto normal = mito::functions::constant<coordinates_t>(
        mito::tensor::vector_t<2>{ inv_sqrt2, -inv_sqrt2 });

    // the interface manifold
    auto interface_manifold = mito::manifolds::submanifold(interface_mesh, coord_system, normal);

    // the degrees of freedom that either side of the interface assigns to the two mesh nodes
    auto left_0 = discretization_node_t();
    auto left_1 = discretization_node_t();
    auto right_0 = discretization_node_t();
    auto right_1 = discretization_node_t();
    auto left_map = node_map_t{ { node_0, left_0 }, { node_1, left_1 } };
    auto right_map = node_map_t{ { node_0, right_0 }, { node_1, right_1 } };

    // the butterfly elements on the interface
    auto interface =
        mito::fem::interface_elements<interface_element_t>(interface_manifold, left_map, right_map);

    // the base P1 mass and stiffness matrices for a unit-length segment
    auto M = 1.0 / 6.0 * mito::tensor::matrix_t<2>{ 2.0, 1.0, 1.0, 2.0 };
    auto G = mito::tensor::matrix_t<2>{ 1.0, -1.0, -1.0, 1.0 };

    // the coefficient fields
    auto one = mito::functions::one<coordinates_t>;
    auto identity = mito::functions::identity<coordinates_t, 2>();

    // the interface blocks
    auto jump_jump_block = mito::fem::blocks::jump_jump<interface_traits_t, doe>(one);
    auto average_average_block =
        mito::fem::blocks::average_average_gradient<interface_traits_t, doe>(identity);

    // there is one interface element
    EXPECT_EQ(1, std::size(interface.elements()));

    for (const auto & element : interface.elements()) {
        // the element couples the left side's degrees of freedom, then the right side's
        EXPECT_EQ(4, std::size(element.connectivity()));
        EXPECT_EQ(left_0, element.connectivity()[0]);
        EXPECT_EQ(left_1, element.connectivity()[1]);
        EXPECT_EQ(right_0, element.connectivity()[2]);
        EXPECT_EQ(right_1, element.connectivity()[3]);

        {
            // the jump [[phi]] = phi_left - phi_right tiles +M on the same-side blocks and -M on
            // the opposite-side blocks
            auto analytical_block = mito::tensor::matrix_t<4>{
                M[{ 0, 0 }],  M[{ 0, 1 }],  -M[{ 0, 0 }], -M[{ 0, 1 }], M[{ 1, 0 }], M[{ 1, 1 }],
                -M[{ 1, 0 }], -M[{ 1, 1 }], -M[{ 0, 0 }], -M[{ 0, 1 }], M[{ 0, 0 }], M[{ 0, 1 }],
                -M[{ 1, 0 }], -M[{ 1, 1 }], M[{ 1, 0 }],  M[{ 1, 1 }],
            };
            auto error = mito::tensor::norm(jump_jump_block.compute(element) - analytical_block);
            // {analytical_block} is reassembled from {M} entries with a different operation order
            // than the quadrature sum, so machine-epsilon-level roundoff is expected
            EXPECT_NEAR(0.0, error, 1.0e-14);
        }

        {
            // the average {phi} = (phi_left + phi_right) / 2 tiles the same 1/4 G everywhere,
            // with no sign distinction between the sides
            auto quarter_G = 0.25 * G;
            auto analytical_block = mito::tensor::matrix_t<4>{
                quarter_G[{ 0, 0 }], quarter_G[{ 0, 1 }], quarter_G[{ 0, 0 }], quarter_G[{ 0, 1 }],
                quarter_G[{ 1, 0 }], quarter_G[{ 1, 1 }], quarter_G[{ 1, 0 }], quarter_G[{ 1, 1 }],
                quarter_G[{ 0, 0 }], quarter_G[{ 0, 1 }], quarter_G[{ 0, 0 }], quarter_G[{ 0, 1 }],
                quarter_G[{ 1, 0 }], quarter_G[{ 1, 1 }], quarter_G[{ 1, 0 }], quarter_G[{ 1, 1 }],
            };
            auto error =
                mito::tensor::norm(average_average_block.compute(element) - analytical_block);
            EXPECT_NEAR(0.0, error, 1.0e-14);
        }
    }

    // all done
    return;
}


// end of file
