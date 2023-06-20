#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>

TEST(Point, TestPoint)
{
    auto cloud = mito::geometry::point_cloud<2>();

    auto point_a = cloud.point({ 0.0, 0.0 });
    EXPECT_EQ(mito::geometry::distance(point_a, point_a), 0.0);

    auto point_b = cloud.point({ 1.0, 1.0 });
    EXPECT_EQ(mito::geometry::distance(point_a, point_b), std::sqrt(2.0));

    // print point cloud
    std::cout << cloud << std::endl;
}