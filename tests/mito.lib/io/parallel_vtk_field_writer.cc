// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito/simulation.h>
#include <mito/mesh.h>
#include <mito/io.h>

// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using mesh_t = mito::mesh::mesh_t<mito::geometry::triangle_t<2>>;


TEST(ParallelVTKWriter, FieldWriter)
{
    // the simulation representative
    auto & simulation = mito::simulation::simulation();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh
    std::ifstream fileStream("disk_cartesian.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

    // number of partitions
    int n_partitions = simulation.context().n_tasks();

    // rank of the mesh to return
    int n_rank = simulation.context().task_id();

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, n_partitions, n_rank);

    // a nodal field on the mesh
    auto nodal_field = mito::fem::nodal_field<mito::vector_t<2>>(mesh_partition, "normal");

    // the normal field to the disk
    constexpr auto normal_field = mito::fields::field([](const coordinates_t & x) -> auto {
        mito::scalar_t theta = std::atan2(x[1], x[0]);
        return std::cos(theta) * mito::e_0<2> + std::sin(theta) * mito::e_1<2>;
    });

    // fill information in nodal field
    for (auto & [node, value] : nodal_field) {
        // get the coordinates of the node
        auto & coordinates = coord_system.coordinates(node->point());
        // compute the value of the normal field at those coordinates
        value = normal_field(coordinates);
    }

    // write mesh to vtk file
    auto writer = mito::io::vtk::parallel_field_writer("disk_field", mesh_partition, coord_system);
    // sign {nodal_field} up with the writer
    writer.record(nodal_field);
    // write output file
    writer.write();

    // all done
    return;
}


// end of file
