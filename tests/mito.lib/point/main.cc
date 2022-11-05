#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>

TEST(Point, TestPoint)
{
    auto point_a = mito::geometry::point(0.0, 0.0);
    EXPECT_EQ(mito::geometry::distance(point_a, point_a), 0.0);

    auto point_b = mito::geometry::point(1.0, 1.0);
    EXPECT_EQ(mito::geometry::distance(point_a, point_b), std::sqrt(2.0));
}