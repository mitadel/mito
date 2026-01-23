// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/manifolds.h>


// cartesian coordinates in 3D
using cartesian_coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;
// spherical coordinates
using spherical_coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::SPHERICAL>;


TEST(Manifolds, Ball)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<cartesian_coordinates_t>();

    // read the mesh of a ball
    std::ifstream fileStream("bottom_half_ball.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // create a manifold on {mesh}
    auto manifold_cartesian = mito::manifolds::manifold(mesh, coord_system);

    // compute the area of the manifold
    auto volume_cartesian = manifold_cartesian.volume();

    // perform change of coordinates from cartesian to spherical
    auto spherical_coord_system =
        mito::geometry::coordinate_system<spherical_coordinates_t>(coord_system);

    // create a manifold on {mesh}
    auto manifold_spherical = mito::manifolds::manifold(mesh, spherical_coord_system);

    // compute the area of the manifold
    auto volume_spherical = manifold_spherical.volume();

    // expect the same result in cartesian and spherical coordinates
    EXPECT_DOUBLE_EQ(volume_cartesian, volume_spherical);

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the boundary of the ball)
    EXPECT_NEAR(4.0 / 3.0 * std::numbers::pi / 2.0, volume_spherical, 0.11);
}
