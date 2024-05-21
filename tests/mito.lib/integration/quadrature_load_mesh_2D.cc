// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the function extracting the {x_0} components of a 2D vector
constexpr auto x_0 = mito::geometry::cartesian::x_0<2>;
// the function extracting the {x_1} components of a 2D vector
constexpr auto x_1 = mito::geometry::cartesian::x_1<2>;


TEST(Quadrature, LoadMeshTriangles)
{
    // make a channel
    journal::info_t channel("tests.quadrature");

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);
    auto manifold = mito::manifolds::manifold(mesh, coord_system);

    // instantiate a scalar field
    auto f = mito::fields::field(mito::functions::cos(x_0 * x_1));

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto result = integrator.integrate(f);
    auto exact = 0.9460830607878437;
    channel << "Integration of cos(x*y): Result = " << result
            << ", Error = " << std::fabs(result - exact) << journal::endl;
    EXPECT_NEAR(result, exact, 1.e-7);
}

// end of file
