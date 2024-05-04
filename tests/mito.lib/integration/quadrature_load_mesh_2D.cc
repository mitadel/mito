// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;


TEST(Quadrature, LoadMeshTriangles)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // instantiate a scalar field
    auto f = mito::fields::field([](const coordinates_t & x) { return std::cos(x[0] * x[1]); });

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f);
    auto exact = 0.9460830607878437;
    std::cout << "Integration of cos(x*y): Result = " << result
              << ", Error = " << std::fabs(result - exact) << std::endl;
    EXPECT_NEAR(result, exact, 1.e-7);
}

// end of file
