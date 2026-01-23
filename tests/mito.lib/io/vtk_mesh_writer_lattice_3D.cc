// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(VtkWriter, MeshLattice3D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read summit mesh
    std::ifstream fileStream("lattice.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::segment_t<3>>(fileStream, coord_system);

    // write mesh to vtk file
    mito::io::vtk::grid_writer("lattice", mesh, coord_system).write();
}