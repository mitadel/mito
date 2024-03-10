// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Director, Segment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // create two nodes
    auto vertex_0 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0 });
    auto vertex_1 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0 });

    // create a segment
    auto segment = topology.segment({ vertex_0, vertex_1 });

    // compute the cell directors
    // get the directors of the tetrahedron
    auto [_, directors] = mito::geometry::directors(segment, geometry, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE((directors[0] == mito::vector_t<2>{ 1.0, 0.0 }));

    // all done
    return;
}

// TOFIX: implement cases for triangle and tetrahedron

// end of file
