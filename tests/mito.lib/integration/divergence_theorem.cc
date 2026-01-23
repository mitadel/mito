// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// strip namespace
using mito::quadrature::GAUSS;
// strip namespace
using mito::tensor::_;
// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the function extracting the {x_0} component of a 2D vector
constexpr auto x0 = mito::geometry::cartesian::x_0<2>;
// the function extracting the {x_1} component of a 2D vector
constexpr auto x1 = mito::geometry::cartesian::x_1<2>;
// the function returning the constant e0 unit vector in 2D
constexpr auto e0 = mito::geometry::cartesian::e_0<2>;
// the function returning the constant e1 unit vector in 2D
constexpr auto e1 = mito::geometry::cartesian::e_1<2>;

TEST(DivergenceTheorem, Mesh2D)
{
    // make a channel
    journal::info_t channel("tests.divergence_theorem");

    // a vector field
    constexpr auto f = x0 * x1 * e0 + x0 * x0 * e1;

    // build a scalar field with divergence of field
    constexpr auto div = mito::fields::divergence(f);

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

    // build nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.5, 0.5 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 1.0 });

    // populate the mesh
    mesh.insert({ node_0, node_1, node_3 });
    mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // create the body manifold
    auto bodyManifold = mito::manifolds::manifold(mesh, coord_system);
    // create the body integrator
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    // the integral of the divergence on the body
    //  int_{int} (div f) = int_0^1 int_0^1 y dx dy = 0.5
    auto resultBody = bodyIntegrator.integrate(div);

    // report
    channel << "Result of body integration = " << resultBody << journal::endl;

    // TOFIX: Include normal notion on the boundary, so that we can avoid hardcoding
    // the normals calculations

    // the normal to the boundary
    constexpr auto n = mito::functions::function([](const coordinates_t & x) {
        // the left, right, bottom and top normals
        return (x[0] == 0.0) * (-e0(x)) + (x[0] == 1.0) * e0(x) + (x[1] == 0.0) * (-e1(x))
             + (x[1] == 1.0) * e1(x);
    });

    // the boundary mesh
    auto boundary_mesh = mito::mesh::boundary(mesh);

    // the boundary manifold
    auto boundary_manifold = mito::manifolds::submanifold(boundary_mesh, coord_system, n);

    // the boundary integrator
    auto boundary_integrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundary_manifold);

    // the integral of the original field on the boundary
    //  int_{right} (f * n) = int_{right} (f0) = int_0^1 xy|(x=1) dy = int_0^1 y dy = 0.5
    //  int_{top} (f * n) = int_{top} (f1) = int_1^0 x^2 dx = int_0^1 x^2 dx = 1/3
    //  int_{left} (f * n) = - int_{left} (f0) = int_0^1 xy|(x=0) dy = 0
    //  int_{bot} (f * n) = - int_{bot} (f1) = - int_0^1 x^2 dx = - 1/3
    auto resultBoundary = boundary_integrator.integrate(f * n);

    // report
    channel << "Result of boundary integration = " << resultBoundary << journal::endl;

    // check that the divergence theorem holds
    EXPECT_DOUBLE_EQ(resultBody, resultBoundary);
}

// end of file
