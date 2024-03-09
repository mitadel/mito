// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


TEST(SummitReader, LoadSummitSegmentsMesh3D)
{
    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    std::ifstream fileStream("segment.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::segment_t<3>>(fileStream, coord_system);

    // assert you read 10 cells
    EXPECT_EQ(mesh.nCells(), 10);

    // assert you found 2 nodes on the boundary
    {
        auto boundary_mesh = mito::mesh::boundary(mesh);
        EXPECT_EQ(boundary_mesh.nCells(), 2);
    }

#if 0    // TOFIX
    // show me the cells
    std::cout << "Initial mesh: " << std::endl;
    int count = 0;
    decltype(mesh)::cells_iterator * segment;
    for (auto & simplex = mesh.cells().begin(); simplex != mesh.cells().end(); ++simplex) {
        ++count;
        if (count == 5) {
            segment = simplex;
        }
    }

    // erase a simplex
    mesh.erase(segment);
    // topology.erase(segment->simplex());

    // assert you read 9 cells
    EXPECT_EQ(mesh.nCells(), 9);

    // // assert you found 4 nodes on the boundary
    // {
    //     auto boundary_mesh = mito::mesh::boundary(mesh);
    //     EXPECT_EQ(boundary_mesh.nCells(), 4);
    // }
#endif
}