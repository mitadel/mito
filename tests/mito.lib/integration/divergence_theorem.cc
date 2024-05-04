// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


// strip namespace
using mito::quadrature::GAUSS;
// strip namespace
using mito::tensor::_;
// alias for cartesian coordinates
using mito::geometry::CARTESIAN;
// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<2, CARTESIAN>;

// the function extracting the x_0 component of 2D vector
constexpr auto x0 = mito::functions::component<coordinates_t, 0>;
// the function extracting the x_1 component of a 2D vector
constexpr auto x1 = mito::functions::component<coordinates_t, 1>;
// the function returning the constant e0 unit vector in 2D
constexpr auto e0 = mito::functions::constant<coordinates_t>(mito::e_0<2>);
// the function returning the constant e1 unit vector in 2D
constexpr auto e1 = mito::functions::constant<coordinates_t>(mito::e_1<2>);
// euclidean metric
constexpr auto metric = mito::fields::identity_tensor_field<coordinates_t, 2>;

TEST(DivergenceTheorem, Mesh2D)
{
    // a scalar function
    constexpr auto f = mito::fields::field(x0 * x1 * e0 + x0 * x0 * e1);

    // build a scalar field with divergence of field
    constexpr auto div = mito::fields::divergence(f);

    // analytic solution
    // int_{int} (div f) = int_0^1 int_0^1 y dx dy = 0.5
    // int_{boundary} (f.n) = int_{right} (f.n) + int_{top} + int_{left} + int_{bot}
    // int_{right} (f.n) = int_{right} (f0) = int_0^1 xy|(x=1) dy = int_0^1 y dy = 0.5
    // int_{top} (f.n) = int_{top} (f1) = int_1^0 x^2 dx = int_0^1 x^2 dx = 1/3
    // int_{left} (f.n) = - int_{left} (f0) = int_0^1 xy|(x=0) dy = 0
    // int_{bot} (f.n) = - int_{bot} (f1) = - int_0^1 x^2 dx = - 1/3

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

    // a Cartesian coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, CARTESIAN>();

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
    auto resultBody = bodyIntegrator.integrate(div);
    // report
    std::cout << "Result of body integration = " << resultBody << std::endl;

    // the 2D metric volume element
    constexpr auto dx = mito::fields::one_form_field(mito::fields::field(e0), metric);
    constexpr auto dy = mito::fields::one_form_field(mito::fields::field(e1), metric);
    constexpr auto w = mito::fields::wedge(dx, dy);

    // TOFIX: Include normal notion on the boundary element set, so that we can avoid hardcoding
    // the normals calculations
    // the normals to the boundary
    constexpr auto n_bot = -e1;
    constexpr auto n_right = e0;
    constexpr auto n_top = e1;
    constexpr auto n_left = -e0;

    // the 1D restrictions of the 2D metric volume element for each of the boundaries
    constexpr auto w_bot = mito::fields::field(
        [w, n_bot](const coordinates_t & x) -> auto { return w(x)(n_bot(x), _); });
    constexpr auto w_right = mito::fields::field(
        [w, n_right](const coordinates_t & x) -> auto { return w(x)(n_right(x), _); });
    constexpr auto w_top = mito::fields::field(
        [w, n_top](const coordinates_t & x) -> auto { return w(x)(n_top(x), _); });
    constexpr auto w_left = mito::fields::field(
        [w, n_left](const coordinates_t & x) -> auto { return w(x)(n_left(x), _); });

    // integrator on the bottom boundary
    auto meshBot = mito::mesh::mesh<mito::geometry::segment_t<2>>();
    meshBot.insert({ node_0, node_1 });
    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto boundaryBot = mito::manifolds::submanifold(meshBot, coord_system, w_bot);
    auto boundaryBotIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryBot);

    // integrator on the right boundary
    auto meshRight = mito::mesh::mesh<mito::geometry::segment_t<2>>();
    meshRight.insert({ node_1, node_2 });
    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto boundaryRight = mito::manifolds::submanifold(meshRight, coord_system, w_right);
    auto boundaryRightIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryRight);

    // integrator on the top boundary
    auto meshTop = mito::mesh::mesh<mito::geometry::segment_t<2>>();
    meshTop.insert({ node_2, node_4 });
    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto boundaryTop = mito::manifolds::submanifold(meshTop, coord_system, w_top);
    auto boundaryTopIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryTop);

    // integrator on the left boundary
    auto meshLeft = mito::mesh::mesh<mito::geometry::segment_t<2>>();
    meshLeft.insert({ node_4, node_0 });
    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto boundaryLeft = mito::manifolds::submanifold(meshLeft, coord_system, w_left);
    auto boundaryLeftIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(boundaryLeft);

    auto resultBoundary =
        boundaryBotIntegrator.integrate(f * n_bot) + boundaryRightIntegrator.integrate(f * n_right)
        + boundaryTopIntegrator.integrate(f * n_top) + boundaryLeftIntegrator.integrate(f * n_left);

    std::cout << "Result of boundary integration = " << resultBoundary << std::endl;

    // assert divergence theorem
    EXPECT_DOUBLE_EQ(resultBody, resultBoundary);
}

// end of file
