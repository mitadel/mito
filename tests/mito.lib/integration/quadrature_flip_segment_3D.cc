// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// strip the namespace
using mito::geometry::CARTESIAN;
using mito::quadrature::GAUSS;
// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, CARTESIAN>;


TEST(Quadrature, FlipSegment)
{
    // make a channel
    journal::info_t channel("tests.integration");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // a segment
    constexpr auto x_0 = mito::geometry::cartesian::coordinates({ 0.0, 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::cartesian::coordinates({ 1.0, 1.0, 1.0 });
    // create nodes
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);
    auto segment0 = mito::geometry::segment<3>({ node_0, node_1 });

    // construct an orthogonal set {v_1}, {v_2}, {v_3} such that {v_1} is parallel to the segment
    constexpr auto v_1 = (x_1 - x_0) / mito::tensor::norm(x_1 - x_0);
    constexpr auto v_2_try = mito::tensor::e_0<3>;
    constexpr auto v_2 =
        (v_2_try - (v_2_try * v_1) * v_1) / mito::tensor::norm(v_2_try - (v_2_try * v_1) * v_1);
    constexpr auto cross = mito::tensor::cross(v_1, v_2);
    constexpr auto v_3 = cross / mito::tensor::norm(cross);

    // the normal and binormal fields to the segment
    constexpr auto normal_field_1 = mito::fields::uniform_field<coordinates_t>(v_2);
    constexpr auto normal_field_2 = mito::fields::uniform_field<coordinates_t>(v_3);

    // the integrand
    auto f = mito::fields::field([](const coordinates_t & x) { return std::cos(x[0] * x[1]); });

    // a mesh with {segment0}
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<3>>();
    mesh.insert(segment0);

    // create a submanifold on {mesh} with the appropriate normal fields
    auto manifold =
        mito::manifolds::submanifold(mesh, coord_system, normal_field_1, normal_field_2);

    // a second degree-of-exactness integrator on the submanifold
    auto integrator = mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(manifold);
    // integrate {f} on the submanifold
    auto result = integrator.integrate(f);

    // integrate the integrand on the opposite of {segment0}
    auto mesh_flip = mito::mesh::mesh<mito::geometry::segment_t<3>>();
    mesh_flip.insert(mito::geometry::flip(segment0));
    auto manifold_flip =
        mito::manifolds::submanifold(mesh_flip, coord_system, normal_field_1, normal_field_2);
    auto integrator_flip = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold_flip);
    auto result_flip = integrator_flip.integrate(f);

    // report
    channel << "result from integration on segment: " << result << journal::newline
            << "result from integration on flipped segment: " << result_flip << journal::endl;

    // expect that the results obtained are opposite
    EXPECT_DOUBLE_EQ(result, -result_flip);
}

// end of file
