// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/simulation.h>


TEST(MetisPartitionerMPI, LoadMesh)
{
    // the simulation representative
    auto & simulation = mito::simulation::simulation();

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

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
        std::cout << "Initial mesh size = " << mesh.nCells() << std::endl;
        std::cout << "Partitioned mesh size = " << global_ncells << std::endl;
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
