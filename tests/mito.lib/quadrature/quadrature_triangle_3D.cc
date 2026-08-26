// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/quadrature.h>


// strip the namespace
using mito::tensor::_;
using mito::quadrature::GAUSS;

// alias for a set of cartesian coordinates in 3D
using coordinates_t = mito::geometry::cartesian<3>::coordinates_t;
// the euclidean metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;


TEST(Quadrature, Triangle3D)
{
    // make a channel
    journal::info_t channel("tests.quadrature_triangle_3D");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // create nodes
    // build nodes for a unit right triangle rigidly rotated out of the xy-plane
    // (the legs {t_1} = (0.6, 0.0, 0.8) and {t_2} = (0.0, 1.0, 0.0) are unit and orthogonal)
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 0.6, 0.0, 0.8 });
    auto node_2 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });

    // make a geometric simplex
    auto triangle = mito::geometry::triangle(node_0, node_1, node_2);

    // a mesh with {triangle}
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<3>>();
    mesh.insert(triangle);

    // the normal field to the triangle: {n} = {t_1} x {t_2} normalized,
    // oriented such that w(n, t_1, t_2) > 0
    auto normal_field =
        mito::functions::constant<coordinates_t>(mito::tensor::vector_t<3>{ -0.8, 0.0, 0.6 });

    // the ambient metric volume form
    constexpr auto w = metric_space_t::w;

    // the restriction of the metric volume form to the triangle
    auto wS = mito::functions::function(
        [w, normal_field](const coordinates_t & x) { return w(x)(normal_field(x), _, _); });

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
    auto manifold = mito::manifolds::manifold(mesh, coord_system, wS);

    // a scalar field
    auto f_one = mito::functions::constant<coordinates_t>(1.0);

    // a zero degree-of-exactness integrator on the submanifold
    auto integrator = mito::quadrature::integrator<GAUSS, 0 /* degree of exactness */>(manifold);

    // integrate {f_one} on the submanifold
    auto result = integrator.integrate(f_one);
    // the exact result
    auto exact = 0.5;
    // report
    channel << "result: " << result << journal::endl;
    channel << "exact: " << exact << journal::endl;
    // expect a decent match with the exact solution
    EXPECT_DOUBLE_EQ(result, exact);
}

// end of file
