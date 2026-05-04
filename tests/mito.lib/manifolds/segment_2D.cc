// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/manifolds.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Manifolds, Segment2D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of segments
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<2>>();

    // construct a segment
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 0.5, 0.5 });
    mesh.insert({ node_0, node_1 });

    // the normal vector to the segment
    constexpr auto v = mito::tensor::vector_t<2>{ 0.5, -0.5 };
    constexpr auto normal_vector = v / mito::tensor::norm(v);
    constexpr auto normal_field = mito::functions::constant<coordinates_t>(normal_vector);

    // create a submanifold on {mesh} with the appropriate normal field
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, normal_field);

    // compute the length of the segment submanifold
    auto length = manifold.volume();

    // check that the length of the segment is correct
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), length);
}


// end of file
