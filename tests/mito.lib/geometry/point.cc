// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Point, TestPoint)
{
    // point cloud
    auto & cloud = mito::geometry::point_cloud<2>();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // create and place two points
    auto point_0 = cloud.point();
    coord_system.place(point_0, { 0.0, 0.0 });

    // TODO: add a test that distance does not depend on coordinate system
    EXPECT_EQ(mito::geometry::distance(point_0, point_0, coord_system), 0.0);

    auto point_1 = cloud.point();
    coord_system.place(point_1, { 1.0, 1.0 });

    EXPECT_DOUBLE_EQ(mito::geometry::distance(point_0, point_1, coord_system), std::sqrt(2.0));

    // print point cloud
    std::cout << cloud << std::endl;
}
