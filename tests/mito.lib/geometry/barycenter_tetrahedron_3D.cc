// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Barycenter, Tetrahedron3D)
{
    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // build four nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });

    // build a tetrahedron embedded in 3D
    auto tetrahedron = mito::geometry::tetrahedron<3>({ node_0, node_1, node_2, node_3 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(tetrahedron, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::coordinates({ 0.25, 0.25, 0.25 }));

    // all done
    return;
}


// end of file
