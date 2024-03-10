// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(Tetra, MultipleSubdivisions)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // load a mesh of triangles
    std::ifstream fileStream("rectangle.summit");
    auto mesh =
        mito::io::summit::reader<mito::topology::triangle_t>(fileStream, geometry, coord_system);

    // count the cells of the mesh
    int cells = mesh.nCells();

    // do one tetra mesh refinement
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, coord_system);
    // assert that the refined mesh has 4 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), 4 * cells);

    // do two tetra mesh refinements on the original mesh
    auto tetra_tetra_mesh_original = mito::mesh::tetra(mesh, geometry, coord_system, 2);
    // assert that the refined mesh has 16 times more elements than the original one
    EXPECT_EQ(tetra_tetra_mesh_original.nCells(), 16 * cells);

    // do a tetra mesh refinement on the refined mesh
    auto tetra_tetra_mesh_refined = mito::mesh::tetra(tetra_mesh, geometry, coord_system);
    // assert that the refined mesh has 16 times more elements than the original one
    EXPECT_EQ(tetra_tetra_mesh_refined.nCells(), 16 * cells);
}
