// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// polar coordinates in 2D
using polar_coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;


TEST(Point, TestPoint)
{
    // point cloud
    auto & cloud = mito::geometry::point_cloud<2>();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // create and place a point
    auto point_0 = cloud.point();
    coord_system.place(point_0, { 1.0, 1.0 });

    // check the distance of a point from itself is zero
    EXPECT_DOUBLE_EQ(mito::geometry::distance(point_0, point_0, coord_system), 0.0);

    // create and place another point
    auto point_1 = cloud.point();
    coord_system.place(point_1, { 2.0, 2.0 });

    // check that the computed distance of the two points is correct
    EXPECT_DOUBLE_EQ(mito::geometry::distance(point_0, point_1, coord_system), std::sqrt(2.0));

    // transform coordinates to a polar coordinate system
    auto polar_coord_system = mito::geometry::coordinate_system<polar_coordinates_t>(coord_system);

    // check the distances do not change
    EXPECT_DOUBLE_EQ(mito::geometry::distance(point_0, point_0, polar_coord_system), 0.0);
    EXPECT_DOUBLE_EQ(
        mito::geometry::distance(point_0, point_1, polar_coord_system), std::sqrt(2.0));
}
