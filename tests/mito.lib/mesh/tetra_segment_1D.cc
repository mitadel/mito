// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>


TEST(Tetra, Segment)
{
    // a Euclidean coordinate system in 1D
    auto coord_system = mito::geometry::coordinate_system<1, mito::geometry::CARTESIAN>();

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
