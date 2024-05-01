// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


TEST(Mesh, Ball)
{
    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // read the mesh of a ball
    std::ifstream fileStream("ball.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // fetch the boundary of the ball (a sphere)
    auto boundary_mesh = mito::mesh::boundary(mesh);

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::io::vtk::writer("sphere", boundary_mesh, coord_system);
#endif

    // fetch the boundary of the sphere
    auto boundary_boundary_mesh = mito::mesh::boundary(boundary_mesh);

    // check that the boundary of a sphere is an empty mesh (the empty set)
    EXPECT_EQ(0, boundary_boundary_mesh.nCells());
}
