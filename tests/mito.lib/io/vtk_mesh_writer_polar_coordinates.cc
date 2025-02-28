// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// polar coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;


TEST(VtkWriter, MeshPolarCoordinates)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read summit mesh
    std::ifstream fileStream("disk_polar.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

    // write mesh to vtk file
    mito::io::vtk::grid_writer("disk_polar", mesh, coord_system).write();
}