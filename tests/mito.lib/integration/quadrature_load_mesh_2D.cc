// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;
using mito::topology::triangle_t;


TEST(Quadrature, LoadMeshTriangles)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh =
        mito::io::summit::reader<mito::topology::triangle_t, 2>(fileStream, geometry, coord_system);
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // instantiate a scalar field
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::EUCLIDEAN>;
    auto f = mito::manifolds::field([](const coordinates_t & x) { return std::cos(x[0] * x[1]); });

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f);
    auto exact = 0.9460830607878437;
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - exact) << std::endl;
    EXPECT_NEAR(result, exact, 1.e-7);
}

// end of file
