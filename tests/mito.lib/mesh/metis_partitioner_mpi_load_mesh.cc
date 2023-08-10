#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>


using geometry_t = mito::geometry::geometry_t<2>;
using mesh_t = mito::mesh::mesh_t<mito::topology::triangle_t, 2>;


TEST(MetisPartitionerMPI, LoadMesh)
{
    // initialize MPI
    MPI_Init(nullptr, nullptr);

    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    int mpi_size;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // load mesh
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::topology::triangle_t, 2>(fileStream, geometry);

    // number of partitions
    int n_partitions = mpi_size;

    // rank of the mesh to return
    int n_rank = mpi_rank;

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, n_partitions, n_rank);

    // the number of cells in this partition
    int local_ncells = mesh_partition.nCells();
    // the global (reduced) number of cells of all partitions
    int global_ncells = 0;
    MPI_Reduce(&local_ncells, &global_ncells, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // report
    if (mpi_rank == 0) {
        std::cout << "Initial mesh size = " << mesh.nCells() << std::endl;
        std::cout << "Partitioned mesh size = " << global_ncells << std::endl;
    }

    // expect that the sum of the number of cells in the partitioned meshes equals that of the
    // original mesh
    if (mpi_rank == 0) {
        EXPECT_EQ(global_ncells, mesh.nCells());
    }

    // finalize MPI
    MPI_Finalize();

    // all done
    return;
}


// end of file
