// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// second order shape functions type
using shape_t = mito::discretization::ShapeTriangleP2;
// the parametric coordinates type
using parametric_coordinates_t = shape_t::reference_element_type::parametric_coordinates_type;


TEST(Fem, ShapeTriangleP2)
{

    // second order shape functions
    auto element = shape_t();

    // node 0 in barycentric coordinates
    auto n0 = parametric_coordinates_t{ 1.0, 0.0 };
    // node 1 in barycentric coordinates
    auto n1 = parametric_coordinates_t{ 0.0, 1.0 };
    // node 2 in barycentric coordinates
    auto n2 = parametric_coordinates_t{ 0.0, 0.0 };
    // node 3 in barycentric coordinates
    auto n3 = parametric_coordinates_t{ 0.5, 0.5 };
    // node 4 in barycentric coordinates
    auto n4 = parametric_coordinates_t{ 0.0, 0.5 };
    // node 5 in barycentric coordinates
    auto n5 = parametric_coordinates_t{ 0.5, 0.0 };

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