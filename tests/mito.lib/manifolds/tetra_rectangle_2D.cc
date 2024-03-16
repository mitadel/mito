// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/manifolds.h>


TEST(Tetra, Rectangle)
{
    // a Cartesian coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::CARTESIAN>();

    // load a mesh of triangles
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

    // do tetra mesh refinements
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, 1);

    // compute the volume of the original mesh
    auto volume_mesh = mito::manifolds::manifold(mesh, coord_system).volume();

    // compute the volume of the refined mesh
    auto volume_tetra_mesh = mito::manifolds::manifold(tetra_mesh, coord_system).volume();

    // assert that the two volumes coincide
    EXPECT_NEAR(volume_mesh, volume_tetra_mesh, 1.e-15);
}
