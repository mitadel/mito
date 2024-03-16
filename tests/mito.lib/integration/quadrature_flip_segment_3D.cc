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


TEST(Quadrature, FlipSegment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 3D
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // a segment
    auto vertex0 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0, 0.0 });
    auto vertex1 = mito::geometry::node(geometry, coord_system, { 1.0, 1.0, 1.0 });
    auto segment0 = topology.segment({ vertex0, vertex1 });

    // the integrand
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
    auto f = mito::manifolds::field([](const coordinates_t & x) { return std::cos(x[0] * x[1]); });

    // integrate the integrand on {segment0}
    auto mesh = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    mesh.insert(segment0);
    auto manifold = mito::manifolds::manifold(mesh, coord_system);
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);
    auto result = integrator.integrate(f);

    // integrate the integrand on the opposite of {segment0}
    auto mesh_flip = mito::mesh::mesh<mito::topology::segment_t>(geometry);
    mesh_flip.insert(topology.flip(segment0));
    auto manifold_flip = mito::manifolds::manifold(mesh_flip, coord_system);
    auto integrator_flip = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold_flip);
    auto result_flip = integrator_flip.integrate(f);

    std::cout << "Integration of cos(x*y): Result = " << result << std::endl;
    std::cout << "Integration of cos(x*y): Result flipped = " << result_flip << std::endl;

    // expect to obtain a minus sign
    EXPECT_DOUBLE_EQ(result, -result_flip);
}

// end of file
