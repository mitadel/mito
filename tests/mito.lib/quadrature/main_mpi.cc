#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/io.h>
#include <mito/math.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>
#include <mito/quadrature.h>

using mito::vector_t;
using mito::quadrature::GAUSS;
using mito::topology::triangle_t;


TEST(Quadrature, QuadratureLoadMeshParallel)
{
    // initialize MPI
    MPI_Init(nullptr, nullptr);

    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    std::cout << "mpi rank " << mpi_rank << std::endl;

    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // load mesh
    std::ifstream fileStream("square.summit");
    auto mesh = mito::io::summit::reader<mito::topology::triangle_t, 2>(fileStream, geometry);

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, mpi_size, mpi_rank);

    // build the manifold on the partitioned mesh
    auto manifold = mito::manifolds::manifold(mesh_partition);

    // instantiate a scalar field
    auto f = mito::math::function([](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); });
    auto f_cosine = mito::math::field(f);

    // instantiate a GAUSS integrator with degree of exactness equal to 2
    auto integrator = mito::quadrature::integrator<mito::quadrature::GAUSS, 2>(manifold);

    auto local_result = integrator.integrate(f_cosine);

    double global_result = 0.0;
    MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (mpi_rank == 0) {
        std::cout << "Integration of cos(x*y): Result = " << global_result
                  << ", Error = " << std::fabs(global_result - 0.946083) << std::endl;

        EXPECT_NEAR(global_result, 0.946083, 1.e-7);
    }

    // finalize MPI
    MPI_Finalize();
}

// end of file
