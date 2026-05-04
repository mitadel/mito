// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


// cartesian coordinates in 1D
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;

// the basis for vector fields
static constexpr auto e_x = mito::tensor::e_0<1>;


auto
area(
    const auto & w, const mito::geometry::coordinate_system_t<coordinates_t> & coordinate_system,
    const mito::geometry::node_t<1> & v0, const mito::geometry::node_t<1> & v1)
    -> mito::tensor::scalar_t
{
    // get vertex coordinates
    auto x0 = coordinate_system.coordinates(v0->point());
    auto x1 = coordinate_system.coordinates(v1->point());

    // build director vectors
    auto director0 = x1 - x0;

    // compute length of segment
    auto length = w(director0);

    // all done
    return length;
}


TEST(Tensor, EuclideanMetric1D)
{
    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dx(e_x) == 1.0);

    // the metric volume element
    constexpr auto w = dx;

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build nodes of a segment
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });

    // check that even permutations of the vertices give a positive area
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_0, node_1), 1.0);

    // check that odd permutations of the vertices give a negative area
    EXPECT_DOUBLE_EQ(area(w, coord_system, node_1, node_0), -1.0);
}


// end of file
