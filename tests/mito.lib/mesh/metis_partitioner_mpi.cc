// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito/simulation.h>
#include <mito/mesh.h>
#include <mito/io.h>

// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using mesh_t = mito::mesh::mesh_t<mito::geometry::triangle_t<2>>;


TEST(MetisPartitionerMPI, Base)
{
    // the simulation representative
    auto & simulation = mito::simulation::simulation();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    /**
     * Mesh with four cells:
        (0,1)           (1,1)
          4               2
          +---------------+
          | .           . |
          |   .       .   |
          |     .   .     |
          |       . 3     |
          |     .   .     |
          |   .       .   |
          | .           . |
          +---------------+
          0               1
        (0,0)           (1,0)
    */

    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.5, 0.5 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // insert triangles in mesh
    mesh.insert({ node_0, node_1, node_3 });
    mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // number of partitions
    int n_partitions = simulation.context().n_tasks();

    // rank of the mesh to return
    int n_rank = simulation.context().task_id();

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, n_partitions, n_rank);

    // expect that the mesh was partitioned equally
    // (this check assumes that the number of cells of the original mesh is divisible by the number
    //  of partitions requested)
    EXPECT_EQ(mesh_partition.nCells(), mesh.nCells() / n_partitions);

#ifdef WITH_PARALLEL_VTK
    // write mesh to vtk file
    mito::io::vtk::parallel_grid_writer("square", mesh_partition, coord_system).write();
#endif

    // all done
    return;
}


// end of file
