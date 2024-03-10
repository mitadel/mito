// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(SummitToVTK, Mesh3D)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a 3D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    // read summit mesh
    std::ifstream fileStream("cube.summit");
    auto mesh =
        mito::io::summit::reader<mito::topology::simplex_t<3>>(fileStream, geometry, coord_system);

    // write mesh to vtk file
    mito::io::vtk::writer("cube_output", mesh, coord_system);
}