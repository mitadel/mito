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
// second order isoparametric triangle
using element_t = mito::discretization::isoparametric_simplex<2, cell_t>::type;
// the barycentric coordinates type
using barycentric_coordinates_t = typename element_t::barycentric_coordinates_type;


TEST(Fem, IsoparametricTriangleP2)
{
    /**
     * The reference triangle in barycentric coordinates:
                (0,0,1)
                  2
            xi_2  +
                  | .
                  |   .
                  |     .
 (1/2, 0, 1/2)  5 +       + 4 (0, 1/2, 1/2)
                  |         .
                  |           .
                  |             .
                  +-------+-------+ xi_1
                  0       3       1
            (1,0,0)   (1/2, 1/2, 0)   (0,1,0)
    */

    // the coordinate system
    auto coord_system = coord_system_t();

    // build nodes
    auto v0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto v1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto v2 = mito::geometry::node(coord_system, { 1.0, 1.0 });

    // make a geometric simplex
    auto geometric_simplex = mito::geometry::triangle<2>({ v0, v1, v2 });

    // build the discretization nodes
    auto node_0 = discretization_node_t();
    auto node_1 = discretization_node_t();
    auto node_2 = discretization_node_t();
    auto node_3 = discretization_node_t();
    auto node_4 = discretization_node_t();
    auto node_5 = discretization_node_t();

    // a finite element
    auto element = element_t(
        geometric_simplex, coord_system, { node_0, node_1, node_2, node_3, node_4, node_5 });

    // node 0 in barycentric coordinates
    auto n0 = barycentric_coordinates_t{ 1.0, 0.0, 0.0 };
    // node 1 in barycentric coordinates
    auto n1 = barycentric_coordinates_t{ 0.0, 1.0, 0.0 };
    // node 2 in barycentric coordinates
    auto n2 = barycentric_coordinates_t{ 0.0, 0.0, 1.0 };
    // node 3 in barycentric coordinates
    auto n3 = barycentric_coordinates_t{ 0.5, 0.5, 0.0 };
    // node 4 in barycentric coordinates
    auto n4 = barycentric_coordinates_t{ 0.0, 0.5, 0.5 };
    // node 5 in barycentric coordinates
    auto n5 = barycentric_coordinates_t{ 0.5, 0.0, 0.5 };

    // the shape functions at node 0
    auto phi_0 = element.shape<0>();
    // check that the shape function at node 0 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_0(n0));
    // check that the shape function at node 1 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_0(n1));
    // check that the shape function at node 2 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_0(n2));
    // check that the shape function at node 3 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_0(n3));
    // check that the shape function at node 4 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_0(n4));
    // check that the shape function at node 5 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_0(n5));

    // the shape functions at node 1
    auto phi_1 = element.shape<1>();
    // check that the shape function at node 0 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_1(n0));
    // check that the shape function at node 1 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_1(n1));
    // check that the shape function at node 2 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_1(n2));
    // check that the shape function at node 3 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_1(n3));
    // check that the shape function at node 4 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_1(n4));
    // check that the shape function at node 5 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_1(n5));

    // the shape functions at node 2
    auto phi_2 = element.shape<2>();
    // check that the shape function at node 0 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_2(n0));
    // check that the shape function at node 1 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_2(n1));
    // check that the shape function at node 2 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_2(n2));
    // check that the shape function at node 3 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_2(n3));
    // check that the shape function at node 4 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_2(n4));
    // check that the shape function at node 5 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_2(n5));

    // the shape functions at node 3
    auto phi_3 = element.shape<3>();
    // check that the shape function at node 0 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_3(n0));
    // check that the shape function at node 1 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_3(n1));
    // check that the shape function at node 2 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_3(n2));
    // check that the shape function at node 3 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_3(n3));
    // check that the shape function at node 4 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_3(n4));
    // check that the shape function at node 5 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_3(n5));

    // the shape functions at node 4
    auto phi_4 = element.shape<4>();
    // check that the shape function at node 0 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_4(n0));
    // check that the shape function at node 1 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_4(n1));
    // check that the shape function at node 2 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_4(n2));
    // check that the shape function at node 3 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_4(n3));
    // check that the shape function at node 4 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_4(n4));
    // check that the shape function at node 5 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_4(n5));

    // the shape functions at node 5
    auto phi_5 = element.shape<5>();
    // check that the shape function at node 0 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_5(n0));
    // check that the shape function at node 1 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_5(n1));
    // check that the shape function at node 2 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_5(n2));
    // check that the shape function at node 3 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_5(n3));
    // check that the shape function at node 4 is 0.0
    EXPECT_DOUBLE_EQ(0.0, phi_5(n4));
    // check that the shape function at node 5 is 1.0
    EXPECT_DOUBLE_EQ(1.0, phi_5(n5));

    // all done
    return;
}