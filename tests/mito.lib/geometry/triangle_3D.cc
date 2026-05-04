// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;


TEST(Geometry, Triangle3D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build nodes
    constexpr auto x_0 = mito::geometry::cartesian::coordinates({ 0.0, 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::cartesian::coordinates({ 1.0, 0.0, 1.0 });
    constexpr auto x_2 = mito::geometry::cartesian::coordinates({ 1.0, 1.0, 1.0 });

    // build nodes of a triangle (counterclockwise order)
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);
    auto node_2 = mito::geometry::node(coord_system, x_2);

    // build triangle with a positive volume (reference triangle)
    auto triangle = mito::geometry::triangle<3>({ node_0, node_1, node_2 });

    // the normal vector to the submanifold
    constexpr auto cross = mito::tensor::cross(x_1 - x_0, x_2 - x_0);
    constexpr auto normal_vector = cross / mito::tensor::norm(cross);

    // get the metric volume form
    constexpr auto w = metric_space_t::w;

    // get the director edges of this triangle and the point where they stem from
    auto [point, directors] = mito::geometry::directors(triangle, coord_system);

    // strip namespace from the placeholder for forms contractions
    using mito::tensor::_;

    // the 1D restriction of the 2D metric volume form to the segment at the origin of the segment
    constexpr auto wS = w(point)(normal_vector, _, _);

    // compute the area of the triangle
    auto area = 1.0 / mito::tensor::factorial<2>() * wS(directors);

    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), area);
}


// end of file
