// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
// the type of coordinate system
using coord_system_t = mito::geometry::coordinate_system_t<coordinates_t>;
// the type of discretization node
using discretization_node_t = mito::discretization::discretization_node_t;
// the type of cell
using cell_t = mito::geometry::triangle_t<2>;
// first order isoparametric triangle
using element_t =
    typename mito::discretization::isoparametric_simplex<1, cell_t, coord_system_t>::type;
// the barycentric coordinates type
using barycentric_coordinates_t = typename element_t::barycentric_coordinates_type;


TEST(Fem, IsoparametricTriangleP1)
{
    /**
     * The reference triangle in barycentic coordinates:
        (0,0,1)
          2
    xi_2  +
          | .
          |   .
          |     .
          |       .
          |         .
          |           .
          |             .
          +---------------+ xi_1
          0               1
        (1,0,0)           (0,1,0)
    */

    // the coordinate system
    auto coord_system = coord_system_t();

    // the origin of the coordinate system
    auto origin = typename coord_system_t::coordinates_type{};

    // build nodes
    auto coord_0 = coordinates_t{ 0.0, 0.0 };
    auto v0 = mito::geometry::node(coord_system, coord_0);
    auto coord_1 = coordinates_t{ 1.0, 0.0 };
    auto v1 = mito::geometry::node(coord_system, coord_1);
    auto coord_2 = coordinates_t{ 1.0, 1.0 };
    auto v2 = mito::geometry::node(coord_system, coord_2);

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::triangle<2>({ v0, v1, v2 });

    // build the discretization nodes
    auto node_0 = discretization_node_t();
    auto node_1 = discretization_node_t();
    auto node_2 = discretization_node_t();

    // a finite element
    auto element = element_t(
        geometric_simplex, { node_0, node_1, node_2 }, coord_0 - origin, coord_1 - origin,
        coord_2 - origin);

    // node 0 in barycentric coordinates
    auto n0 = barycentric_coordinates_t{ 1.0, 0.0, 0.0 };
    // the shape functions at node 0
    auto phi_0 = element.shape(n0);
    // check that the shape function at node 0 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_0.at(node_0));
    // check that the shape function at node 1 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_0.at(node_1));
    // check that the shape function at node 2 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_0.at(node_2));

    // node 1 in barycentric coordinates
    auto n1 = barycentric_coordinates_t{ 0.0, 1.0, 0.0 };
    // the shape functions at node 1
    auto phi_1 = element.shape(n1);
    // check that the shape function at node 0 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_1.at(node_0));
    // check that the shape function at node 1 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_1.at(node_1));
    // check that the shape function at node 2 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_1.at(node_2));

    // node 2 in barycentric coordinates
    auto n2 = barycentric_coordinates_t{ 0.0, 0.0, 1.0 };
    // the shape functions at node 2
    auto phi_2 = element.shape(n2);
    // check that the shape function at node 0 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_2.at(node_0));
    // check that the shape function at node 1 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_2.at(node_1));
    // check that the shape function at node 2 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_2.at(node_2));

    // all done
    return;
}