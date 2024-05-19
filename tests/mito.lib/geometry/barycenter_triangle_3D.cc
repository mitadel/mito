// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Barycenter, Triangle3D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build three nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.5 });
    auto node_2 = mito::geometry::node(coord_system, { 0.5, 0.5, 0.0 });

    // build a triangle embedded in 3D
    auto triangle = mito::geometry::triangle<3>({ node_0, node_1, node_2 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(triangle, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE(
        barycenter == mito::geometry::cartesian::coordinates({ 0.5, 1.0 / 6.0, 1.0 / 6.0 }));

    // all done
    return;
}


// end of file
