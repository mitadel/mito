// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Mesh, Sphere)
{
    {
        // the coordinate system
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        // read a mesh from a summit mesh file
        std::ifstream fileStream("rectangle.summit");
        auto mesh =
            mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

#ifdef WITH_VTK
        // write mesh to vtk file
        mito::io::vtk::grid_writer("rectangle", mesh, coord_system);
#endif

        // write mesh to summit mesh file
        mito::io::summit::writer("rectangle_copy", mesh, coord_system);
    }

    {
        // the coordinate system
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        // read the newly written mesh
        std::ifstream fileStream("rectangle_copy.summit");
        auto mesh =
            mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

#ifdef WITH_VTK
        // write mesh to vtk file
        mito::io::vtk::grid_writer("rectangle_copy", mesh, coord_system);
#endif
    }
}
