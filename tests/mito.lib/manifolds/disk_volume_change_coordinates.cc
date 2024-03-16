// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(Manifolds, Disk)
{
    // a cartesian coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // read the mesh
    auto filestream = std::ifstream("disk_cartesian.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(filestream, coord_system);

    // perform change of coordinates (cartesian to polar coordinates)
    auto coord_system_polar =
        mito::geometry::coordinate_system<mito::geometry::POLAR>(coord_system);

    // create a manifold on {mesh}
    auto manifold = mito::manifolds::manifold(mesh, coord_system_polar);

    // compute the area of the disk
    auto area = manifold.volume();

    // exact area: the area of a disk with unit radius is {pi}
    auto exact = std::numbers::pi;

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the boundary of the disk)
    EXPECT_NEAR(exact, area, 0.01);
}

// end of file
