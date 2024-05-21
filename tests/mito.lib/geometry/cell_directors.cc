// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Director, Segment)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build two nodes
    auto vertex_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto vertex_1 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });

    // build a segment embedded in 3D
    auto segment = mito::geometry::segment<3>({ vertex_0, vertex_1 });

    // compute the cell directors
    // get the directors of the segment
    auto [_, directors] = mito::geometry::directors(segment, coord_system);

    // check that the director is correct
    EXPECT_TRUE((directors[0] == mito::vector_t<3>{ 1.0, 0.0, 0.0 }));
}


TEST(Director, Triangle)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build three nodes
    auto vertex_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto vertex_1 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto vertex_2 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });

    // build a triangle embedded in 3D
    auto triangle = mito::geometry::triangle<3>({ vertex_0, vertex_1, vertex_2 });

    // compute the cell directors
    // get the directors of the triangle
    auto [_, directors] = mito::geometry::directors(triangle, coord_system);

    // check that the directors are correct
    EXPECT_TRUE((directors[0] == mito::vector_t<3>{ 1.0, 0.0, 0.0 }));
    EXPECT_TRUE((directors[1] == mito::vector_t<3>{ 0.0, 1.0, 0.0 }));
}


TEST(Director, Tetrahedron)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build four nodes
    auto vertex_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto vertex_1 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto vertex_2 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });
    auto vertex_3 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });

    // build a tetrahedron embedded in 3D
    auto tetrahedron = mito::geometry::tetrahedron<3>({ vertex_0, vertex_1, vertex_2, vertex_3 });

    // compute the cell directors
    // get the directors of the tetrahedron
    auto [_, directors] = mito::geometry::directors(tetrahedron, coord_system);

    // check that the directors are correct
    EXPECT_TRUE((directors[0] == mito::vector_t<3>{ 1.0, 0.0, 0.0 }));
    EXPECT_TRUE((directors[1] == mito::vector_t<3>{ 0.0, 1.0, 0.0 }));
    EXPECT_TRUE((directors[2] == mito::vector_t<3>{ 0.0, 0.0, 1.0 }));
}


// end of file
