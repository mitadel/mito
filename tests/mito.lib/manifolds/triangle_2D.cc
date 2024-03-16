// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Manifolds, Triangle2D)
{
    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // an empty mesh of triangles
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // create a manifold on {mesh} with Euclidean metric
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // build triangle with a positive volume (reference triangle)
    auto triangle = mito::geometry::triangle<2>({ node_0, node_1, node_2 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(manifold.volume(triangle), 0.5);

    // create a triangle from an even permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_1, node_2, node_0 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(manifold.volume(triangle), 0.5);

    // create a triangle from an even permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_2, node_0, node_1 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(manifold.volume(triangle), 0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_0, node_2, node_1 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(manifold.volume(triangle), -0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_1, node_0, node_2 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(manifold.volume(triangle), -0.5);

    // create a triangle from an odd permutation of the vertices with respect to the reference
    triangle = mito::geometry::triangle<2>({ node_2, node_1, node_0 });
    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(manifold.volume(triangle), -0.5);
}


// end of file
