// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/mesh.h>


// cartesian coordinates in 3D
using cartesian_coordinates_t = mito::geometry::cartesian<3>::coordinates_t;
// spherical coordinates
using spherical_coordinates_t = mito::geometry::spherical::coordinates_t;


TEST(Mesh, HalfBall)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<cartesian_coordinates_t>();

    // read the mesh of a ball
    std::ifstream fileStream("bottom_half_ball.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::tetrahedron_t<3>>(fileStream, coord_system);

    // the metric space
    using cartesian_metric_space_t =
        mito::geometry::euclidean_metric_space<cartesian_coordinates_t>;

    // loop over the mesh cells
    auto volume_cartesian = 0.0;
    for (const auto & cell : mesh.cells()) {
        volume_cartesian += mito::geometry::volume(cell, coord_system, cartesian_metric_space_t::w);
    }

    // perform change of coordinates from cartesian to spherical
    auto spherical_coord_system =
        mito::geometry::coordinate_system<spherical_coordinates_t>(coord_system);

    // the metric space
    using spherical_metric_space_t =
        mito::geometry::euclidean_metric_space<spherical_coordinates_t>;

    // loop over the mesh cells
    auto volume_spherical = 0.0;
    for (const auto & cell : mesh.cells()) {
        volume_spherical +=
            mito::geometry::volume(cell, spherical_coord_system, spherical_metric_space_t::w);
    }
    // expect the same result in cartesian and spherical coordinates
    EXPECT_DOUBLE_EQ(volume_cartesian, volume_spherical);

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the boundary of the ball)
    EXPECT_NEAR(4.0 / 3.0 * std::numbers::pi / 2.0, volume_spherical, 0.11);
}
