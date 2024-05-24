// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/fem.h>
#include <mito/mesh.h>
#include <mito/io.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Fem, NodalFieldSphere)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a sphere
    std::ifstream fileStream("sphere.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, coord_system);

    // say we have 319 nodes
    int n_nodes = 319;

    // a field of mito vectors
    auto field = mito::fem::nodal_field<mito::vector_t<3>>(n_nodes, "field");

    // populate the field
    int i = 0;
    for (auto & vector : field) {
        if (i == 0) {
            vector = mito::vector_t<3>{ 0.0, 0.0, 0.0 };
        } else {
            vector = field(i - 1) + mito::vector_t<3>{ 1.0, 1.0, 1.0 };
        }
        ++i;
    }

#ifdef WITH_VTK
    // write mesh to vtk file
    mito::io::vtk::writer("sphere_culo", mesh, coord_system, field);
#endif
}