// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/geometry.h>


TEST(VtkWriter, WritePointCloudToVtk)
{
    // an empty point cloud
    auto & cloud = mito::geometry::point_cloud<3>();

    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // add points to the point cloud
    const auto point_a = cloud.point();
    coord_system.place(point_a, { 0.0, 0.0, 0.0 });
    const auto point_b = cloud.point();
    coord_system.place(point_b, { 1.0, 1.0, 0.0 });
    const auto point_c = cloud.point();
    coord_system.place(point_c, { 0.0, 1.0, 1.0 });

    // print the point cloud
    mito::io::vtk::writer("point_cloud_output", cloud, coord_system);
}
