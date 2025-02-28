// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito/simulation.h>
#include <mito/mesh.h>
#include <mito/io.h>

// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(ParallelVtkWriter, CloudField)
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

    // a point field on the cloud
    auto point_field =
        mito::discretization::point_field<mito::tensor::vector_t<3>>(cloud, "normal");

    // the normal field to the submanifold
    constexpr auto normal_field = mito::fields::field([](const coordinates_t & x) -> auto {
        mito::tensor::scalar_t phi = std::atan2(x[1], x[0]);
        mito::tensor::scalar_t theta = std::atan2(std::hypot(x[1], x[0]), x[2]);
        return std::sin(theta) * std::cos(phi) * mito::tensor::e_0<3>
             + std::sin(theta) * std::sin(phi) * mito::tensor::e_1<3>
             + std::cos(theta) * mito::tensor::e_2<3>;
    });

    // fill information in point field
    for (auto & [point, value] : point_field) {
        // get the coordinates of the node
        auto & coordinates = coord_system.coordinates(point);
        // compute the value of the normal field at those coordinates
        value = normal_field(coordinates);
        std::cout << coordinates << " " << value << std::endl;
    }

    // write mesh to vtk file
    auto writer = mito::io::vtk::parallel_field_writer("sphere_cloud_field", cloud, coord_system);
    // sign {point_field} up with the writer
    writer.record(point_field);
    // write output file
    writer.write();

    // all done
    return;
}


// end of file
