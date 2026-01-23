// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/quadrature.h>


using mito::tensor::vector_t;
using mito::real;
using mito::quadrature::GAUSS;

// alias for a set of cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the function extracting the {x_0} components of a 2D vector
constexpr auto x_0 = mito::geometry::cartesian::x_0<2>;

// the function extracting the {x_1} components of a 2D vector
constexpr auto x_1 = mito::geometry::cartesian::x_1<2>;


TEST(Quadrature, Square)
{
    // make a channel
    journal::info_t channel("tests.quadrature");

    /**
     * Mesh with four cells:
        (0,1)           (1,1)
          4               2
          +---------------+
          | .           . |
          |   .       .   |
          |     .   .     |
          |       . 3     |
          |     .   .     |
          |   .       .   |
          | .           . |
          +---------------+
          0               1
        (0,0)           (1,0)
    */

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<2>>();

    // create nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.5, 0.5 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // insert triangles in mesh
    mesh.insert({ node_0, node_1, node_3 });
    mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
    auto bodyManifold = mito::manifolds::manifold(mesh, coord_system);
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    // a scalar field
    auto f = mito::functions::cos(x_0 * x_1);
    // integrate the field
    real result = bodyIntegrator.integrate(f);
    // the exact solution
    constexpr auto exact = real{ 0.9460830607878437 };
    // report
    channel << "Integration of cos(x*y): Result = " << result
            << ", Error = " << std::fabs(result - exact) << journal::endl;
    // check the result
    EXPECT_NEAR(result, exact, 1.e-3);

    // a scalar field
    auto f_one = mito::functions::constant<coordinates_t>(1.0);
    // integrate the field
    result = bodyIntegrator.integrate(f_one);    // exact 1.0
    // report
    channel << "Integration of 1: Result = " << result << ", Error = " << std::fabs(result - 1.0)
            << journal::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 1.0);

    // a scalar field
    auto f_linear = x_0;
    // integrate the field
    result = bodyIntegrator.integrate(f_linear);    // exact 0.5
    // report
    channel << "Integration of x: Result = " << result << ", Error = " << std::fabs(result - 0.5)
            << journal::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 0.5);

    // a scalar function
    auto f_xy = x_0 * x_1;
    // integrate the field
    result = bodyIntegrator.integrate(f_xy);    // exact 0.25
    // report
    channel << "Integration of x*y: Result = " << result << ", Error = " << std::fabs(result - 0.25)
            << journal::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 0.25);

    // a scalar function
    auto f_xx = x_0 * x_0;
    // integrate the field
    result = bodyIntegrator.integrate(f_xx);    // exact 1.0 / 3.0
    // report
    channel << "Integration of x*x: Result = " << result
            << ", Error = " << std::fabs(result - 1.0 / 3.0) << journal::endl;
    // check the result
    EXPECT_DOUBLE_EQ(result, 1.0 / 3.0);
}

// end of file
