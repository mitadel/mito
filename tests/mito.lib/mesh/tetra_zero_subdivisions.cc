// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>


TEST(Tetra, ZeroSubdivisions)
{
    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // an empty mesh of tetrahedra in 3D
    auto mesh = mito::mesh::mesh<mito::geometry::tetrahedron_t<3>>();

    // vertices for one single tetrahedron
    auto vertex0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto vertex1 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto vertex2 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });
    auto vertex3 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });

    // insert a tetrahedron in the mesh
    mesh.insert({ vertex0, vertex1, vertex2, vertex3 });

    // generate tetra mesh with 0 subdivisions
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, 0);

    // assert that the refined mesh has the same number of elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), mesh.nCells());
}
