// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito/simulation.h>
#include <mito/discrete.h>
#include <mito/mesh.h>
#include <mito/io.h>

// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(ParallelVtkWriter, MeshField)
{
    // the simulation representative
    auto & simulation = mito::simulation::simulation();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // number of partitions
    int n_partitions = simulation.context().n_tasks();

    // expect 2 partitions
    ASSERT_EQ(2, n_partitions);

    // rank of the mesh to return
    int n_rank = simulation.context().task_id();

    // the mesh file to read
    std::string mesh_file = (n_rank == 0 ? "top_half_sphere.summit" : "bottom_half_sphere.summit");

    // read the mesh
    std::ifstream fileStream(mesh_file);
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, coord_system);

    // a mesh field on the mesh
    auto mesh_field = mito::discrete::mesh_field<mito::tensor::vector_t<3>>(mesh, "normal");

    // the normal field to the ball
    constexpr auto normal_field = mito::functions::function([](const coordinates_t & x) -> auto {
        mito::tensor::scalar_t phi = std::atan2(x[1], x[0]);
        mito::tensor::scalar_t theta = std::atan2(std::hypot(x[1], x[0]), x[2]);
        return std::sin(theta) * std::cos(phi) * mito::tensor::e_0<3>
             + std::sin(theta) * std::sin(phi) * mito::tensor::e_1<3>
             + std::cos(theta) * mito::tensor::e_2<3>;
    });

    // fill information in mesh field
    for (auto & [node, value] : mesh_field) {
        // get the coordinates of the node
        auto & coordinates = coord_system.coordinates(node->point());
        // compute the value of the normal field at those coordinates
        value = normal_field(coordinates);
    }

    // write mesh to vtk file
    auto writer = mito::io::vtk::parallel_field_writer("sphere_mesh_field", mesh, coord_system);
    // sign {mesh_field} up with the writer
    writer.record(mesh_field);
    // write output file
    writer.write();

    // all done
    return;
}


// end of file
