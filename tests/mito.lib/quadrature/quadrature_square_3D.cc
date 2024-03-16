// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/quadrature.h>


using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;

// alias for a set of cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::EUCLIDEAN>;


TEST(Quadrature, Square)
{
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

    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    // an empty mesh of simplicial topology in 3D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<3>>();

    // create nodes
    auto node_0 = mito::geometry::node(coord_system, { 0.0, 0.0, 0.0 });
    auto node_1 = mito::geometry::node(coord_system, { 1.0, 0.0, 1.0 });
    auto node_2 = mito::geometry::node(coord_system, { 1.0, 1.0, 1.0 });
    auto node_3 = mito::geometry::node(coord_system, { 0.5, 0.5, 0.5 });
    auto node_4 = mito::geometry::node(coord_system, { 0.0, 1.0, 0.0 });

    // insert triangles in mesh
    mesh.insert({ node_0, node_1, node_3 });
    mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // instantiate an cell set with the same cells as above but the new coordinates map
    auto bodyManifold = mito::manifolds::manifold(mesh, coord_system);

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
    auto bodyIntegrator =
        mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(bodyManifold);

    // a scalar field
    auto f_xy = mito::manifolds::field([](const coordinates_t & x) -> real { return x[0] * x[1]; });

    // integrate the field
    real result = bodyIntegrator.integrate(f_xy);
    // the exact solution
    constexpr auto exact = real{ 0.35355339059327384 };
    // report
    std::cout << "Integration of x*y in 3D: Result = " << result
              << ", Error = " << std::fabs(result - exact) << std::endl;

    // check the result
    EXPECT_DOUBLE_EQ(result, exact);
}

// end of file
