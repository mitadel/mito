// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/topology.h>
#include <mito/quadrature.h>


TEST(ParametricTetrahedron, Order1)
{
    // a Gauss quadrature rule on tetrahedrons with degree of exactness 1
    constexpr auto quadrature_rule = mito::quadrature::quadrature_rule<
        mito::quadrature::GAUSS, mito::topology::tetrahedron_t, 1>();

    // the parametric point type
    using point_t = decltype(quadrature_rule)::quadrature_point_type;

    // a linear function of the parametric coordinates x_0
    constexpr auto f = [](const point_t & x) -> mito::tensor::scalar_t {
        return x[0];
    };

    // area of the parametric tetrahedron
    constexpr auto area = 1.0 / 3.0;

    // integral of f on the parametric tetrahedron
    constexpr auto integral = area * quadrature_rule.weight(0) * f(quadrature_rule.point(0));

    // exact solution
    constexpr auto exact = 1.0 / 12.0;

    // check result
    static_assert(std::fabs(integral - exact) < 1.e-16);
}

TEST(ParametricTetrahedron, Order2)
{
    // a Gauss quadrature rule on tetrahedrons with degree of exactness 2
    constexpr auto quadrature_rule = mito::quadrature::quadrature_rule<
        mito::quadrature::GAUSS, mito::topology::tetrahedron_t, 2>();

    // the parametric point type
    using point_t = decltype(quadrature_rule)::quadrature_point_type;

    // a quadratic function of the parametric coordinates x_0^2
    constexpr auto f = [](const point_t & x) -> mito::tensor::scalar_t {
        return x[0] * x[0];
    };

    // area of the parametric tetrahedron
    constexpr auto area = 1.0 / 3.0;

    // integral of f on the parametric tetrahedron
    constexpr auto integral = area
                            * (quadrature_rule.weight(0) * f(quadrature_rule.point(0))
                               + quadrature_rule.weight(1) * f(quadrature_rule.point(1))
                               + quadrature_rule.weight(2) * f(quadrature_rule.point(2))
                               + quadrature_rule.weight(3) * f(quadrature_rule.point(3)));

    // exact solution
    constexpr auto exact = 1.0 / 30.0;

    // check result
    static_assert(std::fabs(integral - exact) < 1.e-16);
}

// end of file
