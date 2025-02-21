// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/topology.h>
#include <mito/quadrature.h>


TEST(ParametricTriangle, Order1)
{
    // a Gauss quadrature rule on triangles with degree of exactness 1
    constexpr auto quadrature_rule =
        mito::quadrature::quadrature_rule<mito::quadrature::GAUSS, mito::topology::triangle_t, 1>();

    // the parametric point type
    using point_t = decltype(quadrature_rule)::quadrature_point_type;

    // a linear function of the parametric coordinates x_0
    constexpr auto f = [](const point_t & x) -> mito::tensor::scalar_t {
        return x[0];
    };

    // area of the parametric triangle
    constexpr auto area = 0.5;

    // integral of f on the parametric triangle
    constexpr auto integral = area * quadrature_rule.weight(0) * f(quadrature_rule.point(0));

    // exact solution
    constexpr auto exact = 1.0 / 6.0;

    // check result
    static_assert(std::fabs(integral - exact) < 1.e-16);
}

TEST(ParametricTriangle, Order2)
{
    // a Gauss quadrature rule on triangles with degree of exactness 2
    constexpr auto quadrature_rule =
        mito::quadrature::quadrature_rule<mito::quadrature::GAUSS, mito::topology::triangle_t, 2>();

    // the parametric point type
    using point_t = decltype(quadrature_rule)::quadrature_point_type;

    // a quadratic function of the parametric coordinates x_0^2
    constexpr auto f = [](const point_t & x) -> mito::tensor::scalar_t {
        return x[0] * x[0];
    };

    // area of the parametric triangle
    constexpr auto area = 0.5;

    // integral of f on the parametric triangle
    constexpr auto integral = area
                            * (quadrature_rule.weight(0) * f(quadrature_rule.point(0))
                               + quadrature_rule.weight(1) * f(quadrature_rule.point(1))
                               + quadrature_rule.weight(2) * f(quadrature_rule.point(2)));

    // exact solution
    constexpr auto exact = 1.0 / 12.0;

    // check result
    static_assert(std::fabs(integral - exact) < 1.e-16);
}

TEST(ParametricTriangle, Order3)
{
    // a Gauss quadrature rule on triangles with degree of exactness 3
    constexpr auto quadrature_rule =
        mito::quadrature::quadrature_rule<mito::quadrature::GAUSS, mito::topology::triangle_t, 3>();

    // the parametric point type
    using point_t = decltype(quadrature_rule)::quadrature_point_type;

    // a quadratic function of the parametric coordinates x_0^2
    constexpr auto f = [](const point_t & x) -> mito::tensor::scalar_t {
        return x[0] * x[0] * x[0];
    };

    // area of the parametric triangle
    constexpr auto area = 0.5;

    // integral of f on the parametric triangle [0, 1]
    constexpr auto integral = area
                            * (quadrature_rule.weight(0) * f(quadrature_rule.point(0))
                               + quadrature_rule.weight(1) * f(quadrature_rule.point(1))
                               + quadrature_rule.weight(2) * f(quadrature_rule.point(2))
                               + quadrature_rule.weight(3) * f(quadrature_rule.point(3))
                               + quadrature_rule.weight(4) * f(quadrature_rule.point(4))
                               + quadrature_rule.weight(5) * f(quadrature_rule.point(5)));

    // exact solution (1/3 x^3) at x = 1
    constexpr auto exact = 1.0 / 20.0;

    // check result
    static_assert(std::fabs(integral - exact) < 1.e-16);
}

// end of file
