// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mesh.h>


// cartesian coordinates in 1D
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;


TEST(Tetra, Segment)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<1>>();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });

    // insert cells in the mesh
    mesh.insert({ node_0, node_1 });

    EXPECT_EQ(mesh.nCells(), 1);

    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, 3);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}
