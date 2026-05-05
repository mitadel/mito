// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>
#include <mito/fields.h>
#include <cmath>


using coordinates_1d_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;
using polar_2d_t = mito::geometry::polar_coordinates_t;
using spherical_3d_t = mito::geometry::spherical_coordinates_t;


TEST(Geometry, InducedMetricFromPolar)
{
    // spiral in polar coordinates: (r, θ) = (2ξ, ξ) so r = 2θ
    // J = [2; 1] (constant), g_polar at (2ξ, ξ) = [[1, 0], [0, (2ξ)²]]
    // g_induced = J^T * g_polar * J = 4 + 4ξ² (position-dependent)

    auto xi_0 = mito::functions::component<coordinates_1d_t, 0>;
    constexpr auto e_r = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<0, 2>);
    constexpr auto e_theta = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<1, 2>);
    auto parametrization = 2.0 * xi_0 * e_r + xi_0 * e_theta;

    using ambient_metric_t = mito::geometry::metric<polar_2d_t>;
    auto g_field = mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    // at ξ = 0.5: r = 1, g_induced = [4 + 4·0.25] = [5], volume element = sqrt(5)
    auto xi_1 = mito::geometry::cartesian::coordinates<1>({ 0.5 });
    auto g_val_1 = g_field(xi_1);
    EXPECT_DOUBLE_EQ(5.0, mito::tensor::trace(g_val_1));
    EXPECT_DOUBLE_EQ(std::sqrt(5.0), std::sqrt(mito::tensor::determinant(g_val_1)));

    // at ξ = 1.0: r = 2, g_induced = [4 + 4·1] = [8], volume element = 2·sqrt(2)
    auto xi_2 = mito::geometry::cartesian::coordinates<1>({ 1.0 });
    auto g_val_2 = g_field(xi_2);
    EXPECT_DOUBLE_EQ(8.0, mito::tensor::trace(g_val_2));
    EXPECT_DOUBLE_EQ(2.0 * std::sqrt(2.0), std::sqrt(mito::tensor::determinant(g_val_2)));
}


TEST(Geometry, InducedMetricFromSpherical)
{
    // spiral on a unit sphere: (r, θ, φ) = (1, ξ, 10ξ)
    // J = [0; 1; 10], g_spherical at (1, ξ, 10ξ) = diag(1, 1, sin²(ξ))
    // g_induced = J^T * g * J = 0 + 1 + 100 sin²(ξ) = 1 + 100 sin²(ξ) (position-dependent)

    auto xi_0 = mito::functions::component<coordinates_1d_t, 0>;
    constexpr auto e_r = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<0, 3>);
    constexpr auto e_theta = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<1, 3>);
    constexpr auto e_phi = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<2, 3>);
    auto parametrization = 1.0 * e_r + xi_0 * e_theta + 10.0 * xi_0 * e_phi;

    using ambient_metric_t = mito::geometry::metric<spherical_3d_t>;
    auto g_field = mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    // at ξ = π/6: g = 1 + 100 sin²(π/6) = 1 + 100·(1/4) = 1 + 25 = 26
    auto xi_1 = mito::geometry::cartesian::coordinates<1>({ M_PI / 6.0 });
    auto g_val_1 = g_field(xi_1);
    EXPECT_DOUBLE_EQ(26.0, mito::tensor::trace(g_val_1));
    EXPECT_DOUBLE_EQ(std::sqrt(26.0), std::sqrt(mito::tensor::determinant(g_val_1)));

    // at ξ = π/2: g = 1 + 100 sin²(π/2) = 1 + 100 = 101
    auto xi_2 = mito::geometry::cartesian::coordinates<1>({ M_PI / 2.0 });
    auto g_val_2 = g_field(xi_2);
    EXPECT_DOUBLE_EQ(101.0, mito::tensor::trace(g_val_2));
    EXPECT_DOUBLE_EQ(std::sqrt(101.0), std::sqrt(mito::tensor::determinant(g_val_2)));
}


// end of file
