// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>
#include <mito/tensor.h>


// the basis for vector fields
static constexpr auto e_x = mito::e_0<3>;
static constexpr auto e_y = mito::e_1<3>;
static constexpr auto e_z = mito::e_2<3>;


auto
volume(
    const auto & w,
    const mito::geometry::coordinate_system_t<3, mito::geometry::CARTESIAN> & coordinate_system,
    const mito::geometry::node_t<3> & v0, const mito::geometry::node_t<3> & v1,
    const mito::geometry::node_t<3> & v2, const mito::geometry::node_t<3> & v3) -> mito::scalar_t
{
    // build director segments
    auto x0 = coordinate_system.coordinates(v0.point());
    auto x1 = coordinate_system.coordinates(v1.point());
    auto x2 = coordinate_system.coordinates(v2.point());
    auto x3 = coordinate_system.coordinates(v3.point());

    // build director vectors
    auto director0 = x1 - x0;
    auto director1 = x2 - x0;
    auto director2 = x3 - x0;

    // compute volume of tetrahedron
    auto volume = 1. / 6. * w(director0, director1, director2);

    // all done
    return volume;
}


TEST(Manifolds, EuclideanMetric3D)
{
    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);
    constexpr auto dz = mito::tensor::one_form(e_z);

    // assert that at a(ny) point the basis for one-forms is dual to that of vectors
    static_assert(dx(e_x) == 1.0);
    static_assert(dx(e_y) == 0.0);
    static_assert(dx(e_z) == 0.0);
    static_assert(dy(e_x) == 0.0);
    static_assert(dy(e_y) == 1.0);
    static_assert(dy(e_z) == 0.0);
    static_assert(dz(e_x) == 0.0);
    static_assert(dz(e_y) == 0.0);
    static_assert(dz(e_z) == 1.0);

    // the metric volume element
    constexpr auto w = mito::tensor::wedge(dx, dy, dz);

    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // build nodes of a tetrahedron
    auto node_1 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 0.0, 0.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 0.0, 1.0 });

    // even permutations of the vertices order result in a positive volume
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_1, node_2, node_3, node_4), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_1, node_3, node_4, node_2), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_1, node_4, node_2, node_3), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_2, node_1, node_4, node_3), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_2, node_3, node_1, node_4), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_2, node_4, node_3, node_1), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_3, node_1, node_2, node_4), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_3, node_2, node_4, node_1), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_3, node_4, node_1, node_2), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_4, node_1, node_3, node_2), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_4, node_2, node_1, node_3), 1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_4, node_3, node_2, node_1), 1.0 / 6.0);

    // odd permutations of the vertices order result in a negative volume
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_1, node_2, node_4, node_3), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_1, node_3, node_2, node_4), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_1, node_4, node_3, node_2), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_2, node_1, node_3, node_4), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_2, node_3, node_4, node_1), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_2, node_4, node_1, node_3), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_3, node_1, node_4, node_2), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_3, node_2, node_1, node_4), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_3, node_4, node_2, node_1), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_4, node_1, node_2, node_3), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_4, node_2, node_3, node_1), -1.0 / 6.0);
    EXPECT_DOUBLE_EQ(volume(w, coord_system, node_4, node_3, node_1, node_2), -1.0 / 6.0);
}


// end of file
