// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/quadrature.h>


// cartesian coordinates in 1D
using coordinates_t = mito::geometry::coordinates_t<1, mito::geometry::CARTESIAN>;

// strip namespace
using mito::tensor::vector_t;
using mito::real;
using mito::quadrature::GAUSS;

// the function extracting the {x_0} component of a 1D vector
constexpr auto x_0 = mito::geometry::cartesian::x_0<1>;


TEST(Quadrature, Segment)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t<1>>();

    // a mesh of the segment (0, 1)
    auto node_0 = mito::geometry::node(coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0 });
    mesh.insert({ node_0, node_1 });
    auto tetra_mesh = mito::mesh::tetra(mesh, coord_system, 12);

    // an integrator with degree of exactness 2 on segment (0, 1)
    auto manifold = mito::manifolds::manifold(tetra_mesh, coord_system);
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    // a scalar function
    auto f_exp = mito::functions::exp(-x_0);

    // integrate exp(-x) on (0, 1)
    auto integral = integrator.integrate(f_exp);
    EXPECT_NEAR(integral, (std::exp(1) - 1) / std::exp(1), 1.e-13);
}

// end of file
