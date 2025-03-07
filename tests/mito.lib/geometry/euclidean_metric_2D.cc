// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the basis for vector fields
static constexpr auto e_x = mito::tensor::e_0<2>;
static constexpr auto e_y = mito::tensor::e_1<2>;


auto
area(
    const auto & w, const mito::geometry::coordinate_system_t<coordinates_t> & coordinate_system,
    const mito::geometry::node_t<2> & v0, const mito::geometry::node_t<2> & v1,
    const mito::geometry::node_t<2> & v2) -> mito::tensor::scalar_t
{
    // get vertex coordinates
    auto x0 = coordinate_system.coordinates(v0->point());
    auto x1 = coordinate_system.coordinates(v1->point());
    auto x2 = coordinate_system.coordinates(v2->point());

    // build director vectors
    auto director0 = x1 - x0;
    auto director1 = x2 - x0;

    // compute volume of triangle
    auto area = 1. / 2. * w(director0, director1);

    // all done
    return area;
}


TEST(Tensor, EuclideanMetric2D)
{
    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dx(e_x) == 1.0);
    static_assert(dx(e_y) == 0.0);
    static_assert(dy(e_x) == 0.0);
    static_assert(dy(e_y) == 1.0);

    // the metric volume element
    constexpr auto w = mito::tensor::wedge(dx, dy);

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build nodes of a triangle (counterclockwise order)
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // check that even permutations of the vertices give a positive area
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_0, node_1, node_2), 0.5);
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_1, node_2, node_0), 0.5);
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_2, node_0, node_1), 0.5);

    // check that odd permutations of the vertices give a negative area
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_0, node_2, node_1), -0.5);
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_2, node_1, node_0), -0.5);
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_1, node_0, node_2), -0.5);
}


// end of file
