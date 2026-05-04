// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mesh.h>
#include <mito/io.h>
#include <mito/manifolds.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;


TEST(Tetra, Cube)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the cube mesh
    std::ifstream fileStream("cube.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // do tetra mesh refinement
    const auto subdivisions = 2;
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, subdivisions);

    // check that the refined mesh has 8 times more elements than the original one
    EXPECT_EQ(tetra_mesh.nCells(), std::pow(8, subdivisions) * mesh.nCells());

    // loop over the mesh cells
    auto volume = 0.0;
    for (const auto & cell : tetra_mesh.cells()) {
        volume += mito::geometry::volume(cell, coord_system, metric_space_t::w);
    }

    // check that the result of the calculation is correct
    EXPECT_NEAR(volume, 1.0, 1e-13);
}
