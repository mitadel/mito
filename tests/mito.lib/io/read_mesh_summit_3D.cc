// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(SummitReader, LoadMesh3D)
{
    // make a channel
    journal::info_t channel("tests.read_mesh");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("cube.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);
    channel << "Loaded mesh in " << clock() - t << journal::endl;
}