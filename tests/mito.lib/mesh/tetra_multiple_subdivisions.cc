// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


TEST(Tetra, MultipleSubdivisions)
{
    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // load a mesh of triangles
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

    // count the cells of the mesh
    int cells = mesh.nCells();

    // do one tetra mesh refinement
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system);
    // assert that the refined mesh has 4 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), 4 * cells);

    // do two tetra mesh refinements on the original mesh
    auto tetra_tetra_mesh_original = mito::mesh::tetra(mesh, coord_system, 2);
    // assert that the refined mesh has 16 times more elements than the original one
    EXPECT_EQ(tetra_tetra_mesh_original.nCells(), 16 * cells);

    // do a tetra mesh refinement on the refined mesh
    auto tetra_tetra_mesh_refined = mito::mesh::tetra(tetra_mesh, coord_system);
    // assert that the refined mesh has 16 times more elements than the original one
    EXPECT_EQ(tetra_tetra_mesh_refined.nCells(), 16 * cells);
}
