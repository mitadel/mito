// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the metric space type
using metric_space_t = mito::geometry::metric_space<coordinates_t>;


TEST(Geometry, Segment2D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // construct a segment
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 0.5, 0.5 });

    // build segment from the nodes
    auto segment = mito::geometry::segment<2>({ node_0, node_1 });

    // the normal vector to the segment
    constexpr auto v = mito::tensor::vector_t<2>{ 0.5, -0.5 };
    constexpr auto normal_vector = v / mito::tensor::norm(v);

    // get the metric volume form
    constexpr auto w = metric_space_t::w;

    // get the director edges of this segment and the point where they stem from
    auto [point, directors] = mito::geometry::directors(segment, coord_system);

    // strip namespace from the placeholder for forms contractions
    using mito::tensor::_;

    // the 1D restriction of the 2D metric volume form to the segment at the origin of the segment
    constexpr auto wS = w(point)(normal_vector, _);

    // compute the length of the segment as the contraction of the restricted volume form with the
    // cell directors
    auto length = wS(directors);

    // check that the length of the segment is correct
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), length);
}


// end of file
