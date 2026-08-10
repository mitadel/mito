// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>
#include <mito/simulation.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::cartesian<2>::coordinates_t;
// the euclidean metric space type
using metric_space_t = mito::geometry::euclidean_metric_space<coordinates_t>;

// the {x} function in 2D
constexpr auto x_0 = mito::geometry::cartesian<2>::x;
// the {y} function in 2D
constexpr auto x_1 = mito::geometry::cartesian<2>::y;


TEST(Quadrature, LoadMeshTrianglesMPI)
{
    // make a channel
    journal::info_t channel("tests.quadrature");

    // the simulation representative
    auto & simulation = mito::simulation::simulation();

    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<2>>(fileStream, coord_system);

    // number of partitions
    auto n_tasks = simulation.context().n_tasks();

    // rank of the mesh to return
    auto task_id = simulation.context().task_id();

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, n_tasks, task_id);

    // build the manifold on the partitioned mesh
    auto manifold = mito::manifolds::manifold(mesh_partition, coord_system, metric_space_t::w);

    // instantiate a scalar field
    auto f = mito::functions::cos(x_0 * x_1);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto local_result = integrator.integrate(f);

    double global_result = 0.0;
    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (task_id == 0) {
        auto exact = 0.9460830607878437;
        channel << "Integration of cos(x*y): Result = " << global_result
                << ", Error = " << std::fabs(global_result - exact) << journal::endl;

        EXPECT_NEAR(global_result, exact, 1.e-7);
    }
}

// end of file
