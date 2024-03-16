// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(CoordinateSystem, Cartesian)
{
    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a Cartesian coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::CARTESIAN>();

    // create and place two points
    auto point0 = point_cloud.point();
    auto point1 = point_cloud.point();
    coord_system.place(point0, { 0.0, 1.0 });
    coord_system.place(point1, { 1.0, 0.0 });

    EXPECT_EQ(mito::geometry::coordinates({ 0.0, 1.0 }), coord_system.coordinates(point0));
    EXPECT_EQ(mito::geometry::coordinates({ 1.0, 0.0 }), coord_system.coordinates(point1));
}


// end of file
