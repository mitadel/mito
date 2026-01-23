// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


TEST(Metric, Cartesian)
{
    // a point in space
    constexpr auto point = mito::geometry::cartesian::coordinates<2>({ 2.0, 1.0 });

    // the Euclidean metric
    constexpr auto euclidean_metric = mito::geometry::cartesian::metric<2>;

    // check that the metric field at a point is the identity
    static_assert(euclidean_metric(point) == mito::tensor::diagonal_matrix_t<2>({ 1.0, 1.0 }));
}

TEST(Metric, Polar)
{
    // pick a radius
    constexpr auto r = mito::tensor::scalar_t{ 2.0 };

    // a point in space
    constexpr auto point = mito::geometry::polar::coordinates({ r, 3.0 });

    // the polar metric
    constexpr auto polar_metric = mito::geometry::polar::metric;

    // check that the metric field at a point is e_rr + r^2 e_tt
    static_assert(polar_metric(point) == mito::tensor::diagonal_matrix_t<2>({ 1.0, r * r }));
}

TEST(Metric, Spherical)
{
    // pick a radius
    constexpr auto r = mito::tensor::scalar_t{ 2.0 };
    // and an angle
    constexpr auto theta = mito::tensor::scalar_t{ 1.5 };

    // a point in space
    constexpr auto point = mito::geometry::spherical::coordinates({ r, theta, 3.0 });

    // the spherical metric
    constexpr auto spherical_metric = mito::geometry::spherical::metric;

    // check that the metric field at a point is e_rr + r^2 e_tt + r^2 * sin^2(t) * e_pp
    static_assert(
        spherical_metric(point)
        == mito::tensor::diagonal_matrix_t<3>(
            { 1.0, r * r, r * r * std::sin(theta) * std::sin(theta) }));
}


// end of file
