// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>
#include <mito/simulation.h>

using mito::vector_t;
using mito::quadrature::GAUSS;
using mito::topology::triangle_t;


TEST(Quadrature, LoadMeshTrianglesMPI)
{
    // the simulation representative
    auto & simulation = mito::simulation::simulation();

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

    // number of partitions
    auto n_tasks = simulation.context().n_tasks();

    // rank of the mesh to return
    auto task_id = simulation.context().task_id();

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, coord_system, n_tasks, task_id);

    // build the manifold on the partitioned mesh
    auto manifold = mito::manifolds::manifold(mesh_partition, coord_system);

    // instantiate a scalar field
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::EUCLIDEAN>;
    auto f = mito::manifolds::field([](const coordinates_t & x) { return std::cos(x[0] * x[1]); });

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto local_result = integrator.integrate(f);

    double global_result = 0.0;
    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (task_id == 0) {
        auto exact = 0.9460830607878437;
        std::cout << "Integration of cos(x*y): Result = " << global_result
                  << ", Error = " << std::fabs(global_result - exact) << std::endl;

        EXPECT_NEAR(global_result, exact, 1.e-7);
    }
}

// end of file
