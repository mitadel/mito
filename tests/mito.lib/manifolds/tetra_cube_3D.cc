// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/manifolds.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Tetra, Cube)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the cube mesh
    std::ifstream fileStream("cube.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, subdivisions);
    // assert that the refined mesh has 8 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), std::pow(8, subdivisions) * mesh.nCells());

    // compute the volume of the original mesh
    auto volume_mesh = mito::manifolds::manifold(mesh, coord_system).volume();

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::manifolds::manifold(tetra_mesh, coord_system).volume();

    // assert that the two volumes coincide
    EXPECT_NEAR(volume_mesh, volume_tetra_mesh, 1.e-13);
}
