// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Mesh, Ball)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a ball
    std::ifstream fileStream("ball.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // fetch the boundary of the ball (a sphere)
    auto boundary_mesh = mito::mesh::boundary(mesh);

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::io::vtk::grid_writer("sphere", boundary_mesh, coord_system).write();
#endif

    // fetch the boundary of the sphere
    auto boundary_boundary_mesh = mito::mesh::boundary(boundary_mesh);

    // check that the boundary of a sphere is an empty mesh (the empty set)
    EXPECT_EQ(0, boundary_boundary_mesh.nCells());
}
