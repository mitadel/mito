// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>
#include <cmath>
#include <vector>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the basis for vectors
static constexpr auto e_x = mito::tensor::e_0<2>;
static constexpr auto e_y = mito::tensor::e_1<2>;


// the placeholder for empty slots in contractions
using mito::tensor::_;


auto
length(
    const auto & w, const mito::geometry::coordinate_system_t<coordinates_t> & coordinate_system,
    const mito::geometry::node_t<2> & v0,
    const mito::geometry::node_t<2> & v1) -> mito::tensor::scalar_t
{
    // get vertex coordinates
    auto x0 = coordinate_system.coordinates(v0->point());
    auto x1 = coordinate_system.coordinates(v1->point());

    // build director vectors
    auto director0 = x1 - x0;

    // compute volume of triangle
    auto length = w(director0);

    // all done
    return length;
}


TEST(Tensor, EuclideanSubmanifoldMetric2D)
{
    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // pick two sets of coordinates
    constexpr auto x_0 = mito::geometry::cartesian::coordinates({ 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::cartesian::coordinates({ 3.0, 4.0 });

    // the normal vector to the submanifold
    constexpr auto tangent_vector = x_1 - x_0;
    constexpr auto normal_vector =
        mito::tensor::vector_t<2>{ tangent_vector[1], -tangent_vector[0] }
        / mito::tensor::norm(tangent_vector);

    // the 2D metric volume element
    constexpr auto w = mito::tensor::wedge(dx, dy);

    // the 1D restriction of the 2D metric volume element
    constexpr auto wS = w(normal_vector, _);

    // build nodes of a line segment
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);

    // check that the length of the line segment is correct
    EXPECT_DOUBLE_EQ(5.0, length(wS, coord_system, node_0, node_1));

    // check that the odd permutation of the vertices gives a negative length
    EXPECT_DOUBLE_EQ(-5.0, length(wS, coord_system, node_1, node_0));

    // the normal form to the submanifold
    constexpr auto normal_form = mito::tensor::one_form(normal_vector);

    // rebuild the volume form as wV = wedge(normal_form, wS)
    constexpr auto wV = mito::tensor::wedge(normal_form, wS);

    // check that wV coincides with w
    EXPECT_DOUBLE_EQ(w(e_x, e_y), wV(e_x, e_y));
}


// helper to test a single segment: computes the restricted volume form and verifies length
auto
test_segment(
    mito::geometry::coordinate_system_t<coordinates_t> & coord_system, const auto & w,
    const coordinates_t & x0, const coordinates_t & x1, double expected_length) -> void
{
    // compute tangent and normal vectors
    auto tangent_vector = x1 - x0;
    auto tangent_norm = mito::tensor::norm(tangent_vector);

    // skip degenerate segments
    if (tangent_norm < 1.0e-14) {
        return;
    }

    // normal is tangent rotated 90° clockwise: (tx, ty) -> (ty, -tx)
    // this gives positive length for w(normal, tangent)
    auto normal_vector =
        mito::tensor::vector_t<2>{ tangent_vector[1], -tangent_vector[0] } / tangent_norm;

    // the 1D restriction of the 2D metric volume element
    auto wS = w(normal_vector, _);

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, x0);
    auto node_1 = mito::geometry::node(coord_system, x1);

    // check length (positive orientation)
    EXPECT_NEAR(expected_length, length(wS, coord_system, node_0, node_1), 1.0e-14);

    // check length (negative orientation)
    EXPECT_NEAR(-expected_length, length(wS, coord_system, node_1, node_0), 1.0e-14);
}


TEST(Tensor, EuclideanSubmanifoldMetric2DEdgeCases)
{
    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);

    // the 2D metric volume element
    constexpr auto w = mito::tensor::wedge(dx, dy);

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // define test cases: (x0, y0), (x1, y1), expected_length
    // covering various orientations and edge cases
    struct SegmentTestCase {
        double x0, y0, x1, y1;
        double expected_length;
        const char * description;
    };

    std::vector<SegmentTestCase> test_cases = {
        // axis-aligned segments
        { 0.0, 0.0, 1.0, 0.0, 1.0, "horizontal unit segment" },
        { 0.0, 0.0, 0.0, 1.0, 1.0, "vertical unit segment" },

        // 45-degree diagonal
        { 0.0, 0.0, 1.0, 1.0, std::sqrt(2.0), "diagonal" },

        // Pythagorean triple segments
        { 0.0, 0.0, 3.0, 4.0, 5.0, "3-4-5 segment" },
        { 0.0, 0.0, 4.0, 3.0, 5.0, "4-3-5 segment (swapped)" },

        // steep and shallow angles
        { 0.0, 0.0, 0.1, 10.0, std::sqrt(0.01 + 100.0), "nearly vertical" },
        { 0.0, 0.0, 10.0, 0.1, std::sqrt(100.0 + 0.01), "nearly horizontal" },

        // non-origin segment
        { 1.0, 2.0, 4.0, 6.0, 5.0, "3-4-5 offset from origin" },

        // numerical stability at different scales
        { 0.0, 0.0, 1.0e-6, 1.0e-6, std::sqrt(2.0) * 1.0e-6, "tiny segment" },
        { 0.0, 0.0, 300.0, 400.0, 500.0, "large segment" },
    };

    // run all test cases
    for (const auto & tc : test_cases) {
        SCOPED_TRACE(tc.description);
        auto x0 = mito::geometry::cartesian::coordinates({ tc.x0, tc.y0 });
        auto x1 = mito::geometry::cartesian::coordinates({ tc.x1, tc.y1 });
        test_segment(coord_system, w, x0, x1, tc.expected_length);
    }
}


// end of file
