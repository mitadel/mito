// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>


TEST(Tetra, Tetrahedron)
{
    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // an empty mesh of tetrahedra
    auto mesh = mito::mesh::mesh<mito::geometry::tetrahedron_t<3>>();

    // build nodes of a tetrahedron
    auto node_1 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });

    // insert tetrahedron in mesh tetrahedron with a positive volume
    mesh.insert({ node_1, node_2, node_3, node_4 });

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, subdivisions);
    // assert that the refined mesh has 8 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), std::pow(8, subdivisions) * mesh.nCells());
}
