// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Barycenter, Segment1D)
{
    // a Euclidean coordinate system in 1D
    auto coord_system = mito::geometry::coordinate_system<1, mito::geometry::EUCLIDEAN>();

    // build two nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });

    // build a segment embedded in 1D
    // TOFIX: why can't D = 1 be deduced from the nodes?
    auto segment = mito::geometry::segment<1>({ node_0, node_1 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(segment, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::coordinates({ 0.5 }));

    // all done
    return;
}


// end of file
