// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;


TEST(Barycenter, Segment1D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build two nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });

    // build a segment embedded in 1D
    auto segment = mito::geometry::segment<1>({ node_0, node_1 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(segment, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::cartesian::coordinates({ 0.5 }));

    // all done
    return;
}


// end of file
