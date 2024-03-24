// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/manifolds.h>


// strip the namespace
using mito::geometry::SPHERICAL;
using mito::geometry::CARTESIAN;


TEST(Manifolds, Ball)
{
    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, CARTESIAN>();

    // read the mesh of a ball
    std::ifstream fileStream("half_ball.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // perform change of coordinates from {CARTESIAN} to {SPHERICAL}
    auto spherical_coord_system = mito::geometry::coordinate_system<SPHERICAL>(coord_system);

    // create a manifold on {mesh}
    auto manifold = mito::manifolds::manifold(mesh, spherical_coord_system);

    // compute the area of the manifold
    auto volume = manifold.volume();

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the boundary of the ball)
    EXPECT_NEAR(4.0 / 3.0 * std::numbers::pi / 2.0, volume, 0.11);
}
