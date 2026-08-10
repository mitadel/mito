// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/quadrature.h>


// strip the namespace
using mito::tensor::_;
using mito::quadrature::GAUSS;

// alias for a set of cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the euclidean metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;


TEST(Quadrature, Segment2D)
{
    // make a channel
    journal::info_t channel("tests.quadrature_segment_2D");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // build nodes for a diagonal segment from (0,0) to (3,4) - length 5
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 3.0, 4.0 });

    // make a geometric simplex
    auto segment = mito::geometry::segment(node_0, node_1);

    // a mesh with {segment}
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<2>>();
    mesh.insert(segment);

    // the normal field to the segment (perpendicular to the segment direction)
    // segment direction is (3,4)/5 = (0.6, 0.8), so normal is (0.8, -0.6)
    // (rotated 90° clockwise to get positive orientation with w(normal, tangent) > 0)
    auto normal_field =
        mito::functions::constant<coordinates_t>(mito::tensor::vector_t<2>{ 0.8, -0.6 });

    // the ambient metric volume form
    constexpr auto w = metric_space_t::w;

    // the restriction of the metric volume form to the segment
    auto wS = mito::functions::function(
        [w, normal_field](const coordinates_t & x) { return w(x)(normal_field(x), _); });

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
    auto manifold = mito::manifolds::manifold(mesh, coord_system, wS);

    // a scalar field
    auto f_one = mito::functions::constant<coordinates_t>(1.0);

    // a zero degree-of-exactness integrator on the submanifold
    auto integrator = mito::quadrature::integrator<GAUSS, 0 /* degree of exactness */>(manifold);

    // integrate {f_one} on the submanifold
    auto result = integrator.integrate(f_one);
    // the exact result
    auto exact = 5.0;
    // report
    channel << "result: " << result << journal::endl;
    channel << "exact: " << exact << journal::endl;
    // expect a decent match with the exact solution
    EXPECT_DOUBLE_EQ(result, exact);
}

// end of file
