// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;


TEST(Geometry, Triangle2D)
{
    // the metric volume form
    constexpr auto w = metric_space_t::w;

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // build triangle with a positive volume (reference triangle)
    auto triangle = mito::geometry::triangle<2>({ node_0, node_1, node_2 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(mito::geometry::volume(triangle, coord_system, w), 0.5);

    // create a triangle from an even permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_1, node_2, node_0 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(mito::geometry::volume(triangle, coord_system, w), 0.5);

    // create a triangle from an even permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_2, node_0, node_1 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(mito::geometry::volume(triangle, coord_system, w), 0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_0, node_2, node_1 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(mito::geometry::volume(triangle, coord_system, w), -0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_1, node_0, node_2 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(mito::geometry::volume(triangle, coord_system, w), -0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_2, node_1, node_0 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(mito::geometry::volume(triangle, coord_system, w), -0.5);
}


// end of file
