// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/quadrature.h>


// strip the namespace
using mito::geometry::CARTESIAN;
using mito::quadrature::GAUSS;
// the placeholder for empty slots in contractions
using mito::tensor::_;
// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, CARTESIAN>;


// the basis for vector fields
static constexpr auto e_x = mito::e_0<3>;
static constexpr auto e_y = mito::e_1<3>;
static constexpr auto e_z = mito::e_2<3>;


TEST(Quadrature, Segment3D)
{
    // make a channel
    pyre::journal::info_t channel("tests.quadrature");

    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, CARTESIAN>();

    // a segment
    constexpr auto x_0 = mito::geometry::coordinates({ 0.0, 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::coordinates({ 1.0, 1.0, 1.0 });
    // create nodes
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);
    auto segment0 = mito::geometry::segment<3>({ node_0, node_1 });

    // construct an orthogonal set {v_1}, {v_2}, {v_3} such that {v_1} is parallel to the segment
    constexpr auto v_1 = (x_1 - x_0) / pyre::tensor::norm(x_1 - x_0);
    constexpr auto v_2_try = e_x;
    constexpr auto v_2 =
        (v_2_try - (v_2_try * v_1) * v_1) / pyre::tensor::norm(v_2_try - (v_2_try * v_1) * v_1);
    constexpr auto cross = pyre::tensor::cross(v_1, v_2);
    constexpr auto v_3 = cross / pyre::tensor::norm(cross);

    // the integrand
    auto f = mito::fields::field([](const coordinates_t & x) { return x[0] * x[1]; });

    // a mesh with {segment0}
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<3>>();
    mesh.insert(segment0);

    // the basis one-forms
    constexpr auto dx = mito::tensor::one_form(e_x);
    constexpr auto dy = mito::tensor::one_form(e_y);
    constexpr auto dz = mito::tensor::one_form(e_z);

    // the 3D metric volume element
    constexpr auto w = mito::tensor::wedge(dx, dy, dz);

    // the 1D restriction of the 3D metric volume element
    constexpr auto wS = mito::fields::field(
        [w, v_2, v_3](const coordinates_t &) -> auto { return w(v_2, v_3, _); });

    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, wS);

    // a second degree-of-exactness integrator on the submanifold
    auto integrator = mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(manifold);

    // integrate {f} on the submanifold
    auto result = integrator.integrate(f);

    // the exact result (from mathematica notebook)
    auto exact = 0.5773502691896265;

    // report
    channel << "result: " << result << pyre::journal::endl;
    channel << "exact: " << exact << pyre::journal::endl;

    // expect a decent match with the exact solution
    EXPECT_NEAR(exact, result, 1.e-15);
}

// end of file
