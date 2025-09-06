// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


#include <gtest/gtest.h>
#include <mito/simulation.h>
#include <mito/discrete.h>
#include <mito/mesh.h>
#include <mito/io.h>

// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(ParallelVtkWriter, Mesh)
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
    std::string mesh_file = (n_rank == 0 ? "top_half_ball.summit" : "bottom_half_ball.summit");

    // read the mesh
    std::ifstream fileStream(mesh_file);
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // write mesh to vtk file
    mito::io::vtk::parallel_grid_writer("ball_mesh", mesh, coord_system).write();

    // all done
    return;
}


// end of file
