// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


template <mito::io::summit::GalerkinMeshType galerkinT>
void
test()
{
    int original_mesh_cells = 0;
    int reread_mesh_cells = 0;
    int original_mesh_nodes = 0;
    int reread_mesh_nodes = 0;

    {
        // the coordinate system
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        // read summit mesh
        std::ifstream fileStream("rectangle.summit");
        auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>, galerkinT>(
            fileStream, coord_system);

        // get the original number of mesh cells
        original_mesh_cells = mesh.nCells();

        // get the original number of mesh nodes by counting the nodes of a nodal field built on it
        original_mesh_nodes =
            mito::fem::nodal_field<mito::tensor::scalar_t>(mesh, "field").n_nodes();

        // write summit mesh
        mito::io::summit::writer("rectangle_copy", mesh, coord_system);
    }
    {
        // the coordinate system
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        // read the written summit mesh
        std::ifstream fileStream("rectangle_copy.summit");
        auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>, galerkinT>(
            fileStream, coord_system);

        // get the reread number of mesh cells
        reread_mesh_cells = mesh.nCells();

        // get the reread number of mesh nodes by counting the nodes of a nodal field built on it
        reread_mesh_nodes = mito::fem::nodal_field<mito::tensor::scalar_t>(mesh, "field").n_nodes();

#ifdef WITH_VTK
        // write mesh to vtk file
        mito::io::vtk::grid_writer("rectangle_copy", mesh, coord_system).write();
#endif
    }

    // check that the number of mesh cells is the same
    EXPECT_TRUE(original_mesh_cells == reread_mesh_cells);

    // check that the number of mesh nodes is the same
    EXPECT_TRUE(original_mesh_nodes == reread_mesh_nodes);
}

TEST(SummitToSummit, Mesh2D_CG)
{
    test<mito::io::summit::CG>();
}

TEST(SummitToSummit, Mesh2D_DG)
{
    test<mito::io::summit::DG>();
}