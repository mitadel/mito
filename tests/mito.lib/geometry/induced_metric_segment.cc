// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>
#include <mito/fields.h>
#include <cmath>


using coordinates_1d_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;
using coordinates_2d_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using coordinates_3d_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Geometry, InducedMetricSegment1D)
{
    // unit segment in 1D: x(xi) = 3*xi, J = [3], g_induced = [9],
    // volume_element = 3 (equals length of segment)

    auto xi_0 = mito::functions::component<coordinates_1d_t, 0>;
    constexpr auto e_0 = mito::geometry::basis<coordinates_1d_t>::template e<0>();
    auto parametrization = 3.0 * xi_0 * e_0;

    using ambient_metric_t = mito::geometry::metric<coordinates_1d_t>;
    auto g_field = mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    auto xi = mito::geometry::cartesian::coordinates<1>({ 0.5 });

    // evaluate the metric at xi (1D pullback collapses to scalar: g = J^2 = 9)
    auto g_val = g_field(xi);
    EXPECT_DOUBLE_EQ(9.0, mito::tensor::trace(g_val));

    // check volume element equals sqrt(det(g)): segment length = 3
    EXPECT_DOUBLE_EQ(3.0, std::sqrt(mito::tensor::determinant(g_val)));
}


TEST(Geometry, InducedMetricSegment2D)
{
    // diagonal segment from (0,0) to (3,4): x(xi) = (3*xi, 4*xi), J = [3; 4], g_induced = [25],
    // volume element = 5 (equals length of segment)

    auto xi_0 = mito::functions::component<coordinates_1d_t, 0>;
    constexpr auto e_0 = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<0, 2>);
    constexpr auto e_1 = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<1, 2>);
    auto parametrization = 3.0 * xi_0 * e_0 + 4.0 * xi_0 * e_1;

    using ambient_metric_t = mito::geometry::metric<coordinates_2d_t>;
    auto g_field = mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    auto xi = mito::geometry::cartesian::coordinates<1>({ 0.5 });

    // evaluate the metric tensor at xi
    auto g_val = g_field(xi);
    EXPECT_DOUBLE_EQ(25.0, mito::tensor::trace(g_val));

    // check volume element equals sqrt(det(g)): segment length = 5
    EXPECT_DOUBLE_EQ(5.0, std::sqrt(mito::tensor::determinant(g_val)));
}


TEST(Geometry, InducedMetricSegment2D_CircularArc)
{
    // circular arc in 2D: x(t) = (R cos(t), R sin(t)), R = 3
    // J(t) = [-R sin(t); R cos(t)], g = J^T * I * J = R² (constant despite varying Jacobian)
    // volume element = R = 3

    auto t = mito::functions::component<coordinates_1d_t, 0>;
    constexpr auto e_x = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<0, 2>);
    constexpr auto e_y = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<1, 2>);
    auto parametrization =
        3.0 * mito::functions::cos(t) * e_x + 3.0 * mito::functions::sin(t) * e_y;

    using ambient_metric_t = mito::geometry::metric<coordinates_2d_t>;
    auto g_field = mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    // evaluate at two different points to confirm the field is position-independent
    auto xi_1 = mito::geometry::cartesian::coordinates<1>({ M_PI / 6.0 });
    auto g_val_1 = g_field(xi_1);
    EXPECT_DOUBLE_EQ(9.0, mito::tensor::trace(g_val_1));
    EXPECT_DOUBLE_EQ(3.0, std::sqrt(mito::tensor::determinant(g_val_1)));

    auto xi_2 = mito::geometry::cartesian::coordinates<1>({ M_PI / 3.0 });
    auto g_val_2 = g_field(xi_2);
    EXPECT_DOUBLE_EQ(9.0, mito::tensor::trace(g_val_2));
    EXPECT_DOUBLE_EQ(3.0, std::sqrt(mito::tensor::determinant(g_val_2)));
}

TEST(Geometry, InducedMetricSegment3D)
{
    // segment in 3D from (0,0,0) to (1,1,1): x(xi) = (xi, xi, xi), J = [1;1;1], g_induced = [3],
    // volume_element = sqrt(3) (equals length of segment)

    auto xi_0 = mito::functions::component<coordinates_1d_t, 0>;
    constexpr auto e_0 = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<0, 3>);
    constexpr auto e_1 = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<1, 3>);
    constexpr auto e_2 = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<2, 3>);
    auto parametrization = xi_0 * e_0 + xi_0 * e_1 + xi_0 * e_2;

    using ambient_metric_t = mito::geometry::metric<coordinates_3d_t>;
    auto g_field = mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    auto xi = mito::geometry::cartesian::coordinates<1>({ 0.25 });

    // evaluate the metric tensor at xi
    auto g_val = g_field(xi);
    EXPECT_DOUBLE_EQ(3.0, mito::tensor::trace(g_val));

    // check volume element equals sqrt(det(g)): segment length = sqrt(3)
    EXPECT_DOUBLE_EQ(std::sqrt(3.0), std::sqrt(mito::tensor::determinant(g_val)));
}

// end of file
