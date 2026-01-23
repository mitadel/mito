// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/quadrature.h>


// the type of quadrature
using mito::quadrature::GAUSS;
// the reference segment type
using reference_segment_t = mito::geometry::reference_segment_t;


TEST(ParametricSegment, Order1)
{
    // a Gauss quadrature rule on segments with degree of exactness 1
    constexpr auto quadrature_rule =
        mito::quadrature::quadrature_rule<GAUSS, reference_segment_t, 1>();

    // the parametric point type
    using point_t = decltype(quadrature_rule)::quadrature_point_type;

    // a linear function of the parametric coordinates x_0
    constexpr auto f = [](const point_t & x) -> mito::tensor::scalar_t {
        return x[0];
    };

    // integral of f on the parametric segment [0, 1]
    constexpr auto integral = quadrature_rule.weight(0) * f(quadrature_rule.point(0));

    // exact solution (1/2 x^2) at x = 1
    constexpr auto exact = 0.5;

    // check result
    static_assert(std::fabs(integral - exact) < 1.e-16);
}

TEST(ParametricSegment, Order2)
{
    // a Gauss quadrature rule on segments with degree of exactness 2
    constexpr auto quadrature_rule =
        mito::quadrature::quadrature_rule<GAUSS, reference_segment_t, 2>();

    // the parametric point type
    using point_t = decltype(quadrature_rule)::quadrature_point_type;

    // a quadratic function of the parametric coordinates x_0^2
    constexpr auto f = [](const point_t & x) -> mito::tensor::scalar_t {
        return x[0] * x[0];
    };

    // integral of f on the parametric segment [0, 1]
    constexpr auto integral = quadrature_rule.weight(0) * f(quadrature_rule.point(0))
                            + quadrature_rule.weight(1) * f(quadrature_rule.point(1));

    // exact solution (1/3 x^3) at x = 1
    constexpr auto exact = 1.0 / 3.0;

    // check result
    static_assert(std::fabs(integral - exact) < 1.e-16);
}

// end of file
