// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


TEST(Mesh, Sphere)
{
    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // read the mesh of a sphere
    std::ifstream fileStream("sphere.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, coord_system);

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::io::vtk::writer("sphere", mesh, coord_system);
#endif

    // fetch the boundary of the sphere
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // check that the boundary of a sphere is an empty mesh (the empty set)
    EXPECT_EQ(0, boundary_mesh.nCells());
}
