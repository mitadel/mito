// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(SummitReader, LoadSummitSegmentsMesh3D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    std::ifstream fileStream("segment.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::segment_t<3>>(fileStream, coord_system);

    // assert you read 10 cells
    EXPECT_EQ(mesh.nCells(), 10);

    // assert you found 2 nodes on the boundary
    {
        auto boundary_mesh = mito::mesh::boundary(mesh);
        EXPECT_EQ(boundary_mesh.nCells(), 2);
    }

    // show me the cells
    std::cout << "Initial mesh: " << std::endl;
    auto & cell = mesh.cells()[5];

    // erase a simplex
    mesh.erase(cell);

    // assert you read 9 cells
    EXPECT_EQ(mesh.nCells(), 9);

    // assert you found 4 nodes on the boundary
    {
        auto boundary_mesh = mito::mesh::boundary(mesh);
        EXPECT_EQ(boundary_mesh.nCells(), 4);
    }
}