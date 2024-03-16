// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Director, Segment)
{
    // a Cartesian coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::CARTESIAN>();

    // build two nodes
    auto vertex_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto vertex_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });

    // build a segment embedded in 2D
    auto segment = mito::geometry::segment<2>({ vertex_0, vertex_1 });

    // compute the cell directors
    // get the directors of the segment
    auto [_, directors] = mito::geometry::directors(segment, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE((directors[0] == mito::vector_t<2>{ 1.0, 0.0 }));

    // all done
    return;
}

// TOFIX: implement cases for triangle and tetrahedron

// end of file
