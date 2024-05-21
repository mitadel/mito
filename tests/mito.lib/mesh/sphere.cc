// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Mesh, Sphere)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

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
