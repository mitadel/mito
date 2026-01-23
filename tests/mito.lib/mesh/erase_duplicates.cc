// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mesh.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Mesh, EraseDuplicates)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });

    // insert a triangle in mesh
    mesh.insert({ node_0, node_1, node_2 });

    // insert a geometrical duplicate (which is also a topological duplicate)
    mesh.insert({ node_0, node_1, node_2 });

    // expect to find 2 cells in the mesh
    EXPECT_EQ(mesh.nCells(), 2);

    // erase geometrical duplicates
    mesh.erase_geometrical_duplicates();

    // expect to find 1 cell in the mesh
    EXPECT_EQ(mesh.nCells(), 1);

    // insert a geometrical duplicate (which is also a topological duplicate)
    mesh.insert({ node_0, node_1, node_2 });

    // expect to find 2 cells in the mesh
    EXPECT_EQ(mesh.nCells(), 2);

    // erase topological duplicates
    mesh.erase_topological_duplicates();

    // expect to find 1 cell in the mesh
    EXPECT_EQ(mesh.nCells(), 1);

    // insert a topological duplicate (which is not a geometrical duplicate)
    mesh.insert({ node_1, node_0, node_2 });

    // erase geometrical duplicates
    mesh.erase_geometrical_duplicates();

    // expect to find 1 cell in the mesh
    EXPECT_EQ(mesh.nCells(), 2);

    // erase topological duplicates
    mesh.erase_topological_duplicates();

    // expect to find 1 cell in the mesh
    EXPECT_EQ(mesh.nCells(), 1);

    // all done
    return;
}
