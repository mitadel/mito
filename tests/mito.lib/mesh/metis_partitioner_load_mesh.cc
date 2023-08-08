
#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/io.h>


using geometry_t = mito::geometry::geometry_t<2>;
using mesh_t = mito::mesh::mesh_t<mito::topology::triangle_t, 2>;


TEST(MetisPartitioner, LoadMesh)
{
    // an empty topology
    auto topology = mito::topology::topology();

    // an empty cloud of points
    auto point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto geometry = mito::geometry::geometry(topology, point_cloud);

    // load mesh
    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::io::summit::reader<mito::topology::triangle_t, 2>(fileStream, geometry);

    // number of partitions
    int n_partitions = 2;

    // partition the mesh in two and get the first partition
    auto mesh_partition_0 = mito::mesh::metis::partition(mesh, n_partitions, 0 /* n_rank */);

    // partition the mesh in two and get the second partition
    auto mesh_partition_1 = mito::mesh::metis::partition(mesh, n_partitions, 1 /* n_rank */);

    // report
    std::cout << "Initial mesh size = " << mesh.cells().size() << std::endl;
    std::cout << "Partitioned mesh size 0 = " << mesh_partition_0.cells().size() << std::endl;
    std::cout << "Partitioned mesh size 1 = " << mesh_partition_1.cells().size() << std::endl;

    // expect that the sum of the number of cells in the partitioned meshes equals that of the
    // original mesh
    EXPECT_EQ(
        mesh_partition_0.cells().size() + mesh_partition_1.cells().size(), mesh.cells().size());

    // expect that the partition imbalance is a small fraction of the size of the full mesh
    EXPECT_LE(
        std::abs((int) mesh_partition_0.cells().size() - (int) mesh_partition_1.cells().size()),
        0.05 * mesh.cells().size());

    // all done
    return;
}


// end of file
