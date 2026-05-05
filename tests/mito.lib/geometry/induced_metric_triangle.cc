// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>
#include <mito/fields.h>
#include <cmath>


using coordinates_2d_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
using coordinates_3d_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;
using parametric_2d_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Geometry, InducedMetricTriangle2D)
{
    // unit right triangle: vertices (0,0), (1,0), (0,1)
    // parametrization: x = (xi1 - xi0, 1 - xi0), J = [[-1, 1], [-1, 0]]
    // this gives g_induced = J^T * J = [[2, -1], [-1, 1]]

    auto xi_0 = mito::functions::component<parametric_2d_t, 0>;
    auto xi_1 = mito::functions::component<parametric_2d_t, 1>;
    constexpr auto e_0 = mito::geometry::basis<parametric_2d_t>::template e<0>();
    constexpr auto e_1 = mito::geometry::basis<parametric_2d_t>::template e<1>();
    auto parametrization = (xi_1 - xi_0) * e_0 + (1 - xi_0) * e_1;

    // obtain the induced metric tensor field g = J^T * g_ambient * J
    using ambient_metric_t = mito::geometry::metric<coordinates_2d_t>;
    auto g_field =
        mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    // evaluation point
    auto xi = mito::geometry::cartesian::coordinates<2>({ 1.0 / 3.0, 1.0 / 3.0 });

    // evaluate the metric tensor at xi
    auto g_val = g_field(xi);
    EXPECT_DOUBLE_EQ(2.0, (g_val[{ 0, 0 }]));
    EXPECT_DOUBLE_EQ(-1.0, (g_val[{ 0, 1 }]));
    EXPECT_DOUBLE_EQ(-1.0, (g_val[{ 1, 0 }]));
    EXPECT_DOUBLE_EQ(1.0, (g_val[{ 1, 1 }]));

    // check symmetry of metric tensor
    EXPECT_DOUBLE_EQ((g_val[{ 0, 1 }]), (g_val[{ 1, 0 }]));

    // check volume element equals sqrt(det(g)): det = 2*1 - (-1)*(-1) = 1
    EXPECT_DOUBLE_EQ(1.0, std::sqrt(mito::tensor::determinant(g_val)));
}


TEST(Geometry, InducedMetricTriangle3D)
{
    // tilted triangle in 3D: (0,0,0), (1,0,1), (1,1,1), area = 0.5*sqrt(2)
    // parametrization: x = xi_0*(1,0,1) + xi_1*(1,1,1) = (xi_0+xi_1, xi_1, xi_0+xi_1)
    // J = [[1,1],[0,1],[1,1]], g = J^T*J = [[2,2],[2,3]], det(g) = 2

    auto xi_0 = mito::functions::component<parametric_2d_t, 0>;
    auto xi_1 = mito::functions::component<parametric_2d_t, 1>;
    constexpr auto e_0 = mito::functions::constant<parametric_2d_t>(mito::tensor::e<0, 3>);
    constexpr auto e_1 = mito::functions::constant<parametric_2d_t>(mito::tensor::e<1, 3>);
    constexpr auto e_2 = mito::functions::constant<parametric_2d_t>(mito::tensor::e<2, 3>);
    auto parametrization = (xi_0 + xi_1) * e_0 + xi_1 * e_1 + (xi_0 + xi_1) * e_2;

    using ambient_metric_t = mito::geometry::metric<coordinates_3d_t>;
    auto g_field =
        mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    auto xi = mito::geometry::cartesian::coordinates<2>({ 0.25, 0.25 });

    // evaluate the metric tensor at xi
    auto g_val = g_field(xi);
    EXPECT_DOUBLE_EQ(2.0, (g_val[{ 0, 0 }]));
    EXPECT_DOUBLE_EQ(2.0, (g_val[{ 0, 1 }]));
    EXPECT_DOUBLE_EQ(2.0, (g_val[{ 1, 0 }]));
    EXPECT_DOUBLE_EQ(3.0, (g_val[{ 1, 1 }]));

    // check symmetry of metric tensor
    EXPECT_DOUBLE_EQ((g_val[{ 0, 1 }]), (g_val[{ 1, 0 }]));

    // check volume element equals sqrt(det(g)): det = 2*3 - 2*2 = 2
    EXPECT_DOUBLE_EQ(std::sqrt(2.0), std::sqrt(mito::tensor::determinant(g_val)));
}


// end of file
