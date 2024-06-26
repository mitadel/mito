// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/io.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>
#include <mito/simulation.h>


// cartesian coordinates in 2D
using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

// the function extracting the {x_0} components of a 2D vector
constexpr auto x_0 = mito::geometry::cartesian::x_0<2>;
// the function extracting the {x_1} components of a 2D vector
constexpr auto x_1 = mito::geometry::cartesian::x_1<2>;


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
    auto manifold = mito::manifolds::manifold(mesh_partition, coord_system);

    // instantiate a scalar field
    auto f = mito::fields::field(mito::functions::cos(x_0 * x_1));

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
