// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>


TEST(Tetra, Triangle2D)
{
    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::CARTESIAN>();

    // an empty mesh of triangles in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    // build nodes of a triangle (counterclockwise order)
    auto node_1 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // insert triangle in mesh
    mesh.insert({ node_1, node_2, node_3 });

    // do tetra mesh refinements
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, subdivisions);

    // assert that the refined mesh has 4 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), std::pow(4, subdivisions) * mesh.nCells());
}
