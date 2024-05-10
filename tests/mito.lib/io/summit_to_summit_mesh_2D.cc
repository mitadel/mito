// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(SummitToSummit, Mesh2D)
{
    int original_mesh_cells = 0;
    int reread_mesh_cells = 0;

    {
        // the coordinate system
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        // read summit mesh
        std::ifstream fileStream("rectangle.summit");
        auto mesh =
            mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

        std::cout << mesh.nCells() << std::endl;

        // get the original number of mesh cells
        original_mesh_cells = mesh.nCells();

        // write summit mesh
        mito::io::summit::writer("rectangle_copy", mesh, coord_system);
    }
    {
        // the coordinate system
        auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

        // read the written summit mesh
        std::ifstream fileStream("rectangle_copy.summit");
        auto mesh =
            mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

        std::cout << mesh.nCells() << std::endl;

        // get the reread number of mesh cells
        reread_mesh_cells = mesh.nCells();

#ifdef WITH_VTK
        // write mesh to vtk file
        mito::io::vtk::writer("rectangle_copy", mesh, coord_system);
#endif
    }

    // check that the number of mesh cells is the same
    EXPECT_TRUE(original_mesh_cells == reread_mesh_cells);
}