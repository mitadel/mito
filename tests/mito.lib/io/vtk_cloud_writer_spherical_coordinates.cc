// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/geometry.h>


// spherical coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::SPHERICAL>;


TEST(VtkWriter, CloudSphericalCoordinates)
{
    // an empty point cloud
    auto & cloud = mito::geometry::point_cloud<3>();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // number of sampled azimuthal angles in the range [0, 2 * pi]
    int N_theta = 10;
    // total number of sampled polar angles in the range [0, pi]
    int N_phi = 10;

    // radius of the sphere
    auto r = 1.0;

    for (int i = 0; i < N_theta; ++i) {
        // azimuthal angle
        auto theta = 2.0 * std::numbers::pi * i / N_theta;

        for (int j = 0; j < N_phi; ++j) {
            // polar angle
            auto phi = std::numbers::pi * j / N_phi;

            // create a point at (r, theta, phi)
            const auto point = cloud.point();
            coord_system.place(point, { r, phi, theta });
        }
    }

    // write point cloud to vtk file
    mito::io::vtk::grid_writer("sphere_cloud_spherical_coordinates", cloud, coord_system).write();
}
