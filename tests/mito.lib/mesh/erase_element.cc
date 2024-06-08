// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mesh.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Mesh, EraseElement)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // instantiate four nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.5, 0.5 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // an empty mesh of triangles in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();
    auto & geom_cell0 = mesh.insert({ node_0, node_1, node_3 });
    auto & geom_cell1 = mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // assert you read 4 cells
    EXPECT_EQ(mesh.nCells(), 4);

    // assert the boundary is made of 4 cells
    EXPECT_EQ(mito::mesh::boundary_size(mesh), 4);

    // assert that there exists a segment connecting vertex 0 and 1
    EXPECT_TRUE(topology.exists({ node_0->vertex(), node_1->vertex() }));
    // assert that there exists a segment connecting vertex 1 and 3
    EXPECT_TRUE(topology.exists({ node_1->vertex(), node_3->vertex() }));
    // assert that there exists a segment connecting vertex 3 and 0
    EXPECT_TRUE(topology.exists({ node_3->vertex(), node_0->vertex() }));

    // erase a simplex
    mesh.erase(geom_cell0);
    topology.erase(geom_cell0.simplex());

    // assert the mesh has now 3 cells
    EXPECT_EQ(mesh.nCells(), 3);

    // assert the boundary is now made of 5 cells
    EXPECT_EQ(mito::mesh::boundary_size(mesh), 5);

    // assert that a segment connecting vertex 0 and 1 no longer exists in the topology
    EXPECT_FALSE(topology.exists({ node_0->vertex(), node_1->vertex() }));
    // assert that a segment connecting vertex 1 and 3 no longer exists in the topology
    EXPECT_FALSE(topology.exists({ node_1->vertex(), node_3->vertex() }));
    // assert that a segment connecting vertex 3 and 0 no longer exists in the topology
    EXPECT_FALSE(topology.exists({ node_3->vertex(), node_0->vertex() }));

    // check that erasing a cell twice does not result in an error
    mesh.erase(geom_cell0);
    topology.erase(geom_cell0.simplex());

    mesh.erase(geom_cell1);
    topology.erase(geom_cell1.simplex());

    // assert the mesh has now 2 cells
    EXPECT_EQ(mesh.nCells(), 2);

    // assert the boundary is now made of 4 cells
    EXPECT_EQ(mito::mesh::boundary_size(mesh), 4);

    // assert that a segment connecting vertex 1 and 2 no longer exists in the topology
    EXPECT_FALSE(topology.exists({ node_1->vertex(), node_2->vertex() }));
    // assert that a segment connecting vertex 2 and 3 no longer exists in the topology
    EXPECT_FALSE(topology.exists({ node_2->vertex(), node_3->vertex() }));
    // assert that a segment connecting vertex 3 and 1 no longer exists in the topology
    EXPECT_FALSE(topology.exists({ node_3->vertex(), node_1->vertex() }));
}
