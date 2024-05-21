// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/simulation.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(MetisPartitionerMPI, LoadMesh)
{
    // make a channel
    journal::info_t channel("tests.partitioner");

    // the simulation representative
    auto & simulation = mito::simulation::simulation();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // load mesh
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

    // number of partitions
    int n_partitions = simulation.context().n_tasks();

    // rank of the mesh to return
    int n_rank = simulation.context().task_id();

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, n_partitions, n_rank);

    // the number of cells in this partition
    int local_ncells = mesh_partition.nCells();
    // the global (reduced) number of cells of all partitions
    int global_ncells = 0;
    MPI_Reduce(&local_ncells, &global_ncells, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // report
    if (n_rank == 0) {
        channel << "Initial mesh size = " << mesh.nCells() << journal::endl;
        channel << "Partitioned mesh size = " << global_ncells << journal::endl;
    }

    // expect that the sum of the number of cells in the partitioned meshes equals that of the
    // original mesh
    if (n_rank == 0) {
        EXPECT_EQ(global_ncells, mesh.nCells());
    }

    // all done
    return;
}


// end of file
