// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/geometry.h>
#include <mito/simulation.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(ParallelVtkWriter, Cloud)
{
    // the simulation representative
    auto & simulation = mito::simulation::simulation();

    // an empty point cloud
    auto & cloud = mito::geometry::point_cloud<3>();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // number of partitions
    int n_partitions = simulation.context().n_tasks();

    // rank of the mesh to return
    int n_rank = simulation.context().task_id();

    // number of sampled azimuthal angles in the range [0, 2 * pi] per partition
    int N_theta = 5;
    // total number of sampled polar angles in the range [0, pi]
    int N_phi = 10;

    for (int i = 0; i < N_theta; ++i) {
        // azimuthal angle
        auto theta =
            2.0 * std::numbers::pi * (n_partitions * i + n_rank) / (N_theta * n_partitions);

        for (int j = 0; j < N_phi; ++j) {
            // polar angle
            auto phi = std::numbers::pi * j / N_phi;

            // the cartesian coordinates
            auto x = std::sin(phi) * std::cos(theta);
            auto y = std::sin(phi) * std::sin(theta);
            auto z = std::cos(phi);

            // create a point at (x, y, z)
            const auto point = cloud.point();
            coord_system.place(point, { x, y, z });
        }
    }

    // write point cloud to vtk file
    mito::io::vtk::parallel_grid_writer("sphere_cloud", cloud, coord_system).write();
}
