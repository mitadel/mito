#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Point, TestPoint)
{
    auto & cloud = mito::geometry::point_cloud<2>();

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

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
