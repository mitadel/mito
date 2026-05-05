// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/quadrature.h>
#include <mito/geometry.h>
#include <mito/fields.h>
#include <cmath>
#include <numbers>


using coordinates_1d_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;
using coordinates_2d_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Geometry, ArcLengthCircularArc)
{
    // circular arc: x(t) = (R cos(t), R sin(t)), R = 3, t in [0, pi/2]
    // expected arc length = R * pi/2

    constexpr auto R = 3.0;
    constexpr auto theta = std::numbers::pi / 2.0;

    auto t = mito::functions::component<coordinates_1d_t, 0>;
    constexpr auto e_x = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<0, 2>);
    constexpr auto e_y = mito::functions::constant<coordinates_1d_t>(mito::tensor::e<1, 2>);
    auto parametrization =
        R * mito::functions::cos(t) * e_x + R * mito::functions::sin(t) * e_y;

    // pullback of the Euclidean 2D metric: g_pullback = J^T * I * J = R^2
    using ambient_metric_t = mito::geometry::metric<coordinates_2d_t>;
    auto g_field = mito::geometry::pullback_metric<ambient_metric_t>::field(parametrization);

    // 1D mesh in parameter space [0, theta]
    auto coord_system = mito::geometry::coordinate_system<coordinates_1d_t>();
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<1>>();
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { theta });
    mesh.insert({ node_0, node_1 });
    auto refined_mesh = mito::mesh::tetra(mesh, coord_system, 10);

    // manifold with the pullback metric
    auto manifold = mito::manifolds::manifold(refined_mesh, coord_system, g_field);

    // integrate f = 1 to obtain the arc length
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);
    constexpr auto f = mito::functions::one<coordinates_1d_t>;
    auto arc_length = integrator.integrate(f);

    EXPECT_NEAR(R * theta, arc_length, 1.e-10);
}

// end of file
