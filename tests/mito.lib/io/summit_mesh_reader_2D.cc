// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(SummitReader, Mesh2D)
{
    // make a channel
    journal::info_t channel("tests.read_mesh");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);
    channel << "Loaded mesh in " << clock() - t << journal::endl;
}
