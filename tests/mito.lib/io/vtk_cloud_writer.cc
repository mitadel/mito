// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/geometry.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(VtkWriter, Cloud)
{
    // an empty point cloud
    auto & cloud = mito::geometry::point_cloud<3>();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // add points to the point cloud
    const auto point_a = cloud.point();
    coord_system.place(point_a, { 0.0, 0.0, 0.0 });
    const auto point_b = cloud.point();
    coord_system.place(point_b, { 1.0, 1.0, 0.0 });
    const auto point_c = cloud.point();
    coord_system.place(point_c, { 0.0, 1.0, 1.0 });

    // write point cloud to vtk file
    mito::io::vtk::grid_writer("cloud", cloud, coord_system).write();
}
