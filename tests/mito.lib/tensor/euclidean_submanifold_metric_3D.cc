// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>
#include <mito/tensor.h>


// the basis for vectors
static constexpr auto e_x = mito::e_0<3>;
static constexpr auto e_y = mito::e_1<3>;
static constexpr auto e_z = mito::e_2<3>;


// the placeholder for empty slots in contractions
using mito::tensor::_;


auto
area(
    const auto & w,
    const mito::geometry::coordinate_system_t<3, mito::geometry::CARTESIAN> & coordinate_system,
    const mito::geometry::node_t<3> & v0, const mito::geometry::node_t<3> & v1,
    const mito::geometry::node_t<3> & v2) -> mito::scalar_t
{
    // get vertex coordinates
    auto x0 = coordinate_system.coordinates(v0.point());
    auto x1 = coordinate_system.coordinates(v1.point());
    auto x2 = coordinate_system.coordinates(v2.point());

    // build director vectors
    auto director0 = x1 - x0;
    auto director1 = x2 - x0;

    // compute volume of triangle
    auto area = 1. / 2. * w(director0, director1);

    // all done
    return area;
}


TEST(Tensor, EuclideanSubmanifoldMetric3D)
{
    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);
    constexpr auto dz = mito::tensor::one_form(e_z);

    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // pick three sets of coordinates
    constexpr auto x_0 = mito::geometry::coordinates({ 0.0, 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::coordinates({ 1.0, 0.0, 1.0 });
    constexpr auto x_2 = mito::geometry::coordinates({ 1.0, 1.0, 1.0 });

    // the normal vector to the submanifold
    constexpr auto cross = pyre::tensor::cross(x_1 - x_0, x_2 - x_0);
    constexpr auto normal_vector = cross / pyre::tensor::norm(cross);

    // the 3D metric volume element
    constexpr auto w = mito::tensor::wedge(dx, dy, dz);

    // the 2D restriction of the 3D metric volume element
    constexpr auto wS = w(normal_vector, _, _);

    // build nodes of a triangle (counterclockwise order)
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);
    auto node_2 = mito::geometry::node(coord_system, x_2);

    // check that even permutations of the vertices give a positive area
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), area(wS, coord_system, node_0, node_1, node_2));
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), area(wS, coord_system, node_1, node_2, node_0));
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), area(wS, coord_system, node_2, node_0, node_1));

    // check that odd permutations of the vertices give a negative area
    EXPECT_DOUBLE_EQ(-0.5 * std::sqrt(2.0), area(wS, coord_system, node_0, node_2, node_1));
    EXPECT_DOUBLE_EQ(-0.5 * std::sqrt(2.0), area(wS, coord_system, node_2, node_1, node_0));
    EXPECT_DOUBLE_EQ(-0.5 * std::sqrt(2.0), area(wS, coord_system, node_1, node_0, node_2));

    // the normal form to the submanifold
    constexpr auto normal_form = mito::tensor::one_form(normal_vector);

    // rebuild the volume form as wV = wedge(normal_form, wS)
    constexpr auto wV = mito::tensor::wedge(normal_form, wS);

    // check that wV coincides with w
    EXPECT_DOUBLE_EQ(w(e_x, e_y, e_z), wV(e_x, e_y, e_z));
}


// end of file
