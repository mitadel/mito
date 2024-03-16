// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/mesh.h>


TEST(SummitReader, LoadMesh2D)
{
    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::CARTESIAN>();

    clock_t t;

    //
    t = clock();
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);
    std::cout << "Loaded mesh in " << clock() - t << std::endl;

    t = clock();
    auto boundary_mesh = mesh.boundary();
    std::cout << "Fetched boundary in " << clock() - t << std::endl;
}
