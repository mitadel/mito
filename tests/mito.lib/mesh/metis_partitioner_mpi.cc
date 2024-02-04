
#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/simulation.h>

using geometry_t = mito::geometry::geometry_t<2>;
using coord_system_t = mito::geometry::coordinate_system_t<2, mito::geometry::EUCLIDEAN>;
using mesh_t = mito::mesh::mesh_t<mito::topology::triangle_t, 2>;


// TOFIX: factor this function out
auto
build_mesh(geometry_t & geometry, coord_system_t & coord_system, mesh_t & mesh) -> void
{
    /**
     * Mesh with four cells:
        (0,1)           (1,1)
          4               2
          +---------------+
          | .           . |
          |   .       .   |
          |     .   .     |
          |       . 3     |
          |     .   .     |
          |   .       .   |
          | .           . |
          +---------------+
          0               1
        (0,0)           (1,0)
    */

    auto & topology = geometry.topology();

    auto vertex0 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0 });
    auto vertex1 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0 });
    auto vertex2 = mito::geometry::node(geometry, coord_system, { 1.0, 1.0 });
    auto vertex3 = mito::geometry::node(geometry, coord_system, { 0.5, 0.5 });
    auto vertex4 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0 });

    // print the vertices ids in order of construction
    // std::cout << vertex0.id() << "\t" << vertex1.id() << "\t" << vertex2.id() << "\t"
    //           << vertex3.id() << "\t" << vertex4.id() << std::endl;

    auto segment0 = topology.segment({ vertex0, vertex1 });
    auto segment1 = topology.segment({ vertex1, vertex3 });
    auto segment2 = topology.segment({ vertex3, vertex0 });
    auto cell0 = topology.triangle({ segment0, segment1, segment2 });

    auto segment3 = topology.segment({ vertex1, vertex2 });
    auto segment4 = topology.segment({ vertex2, vertex3 });
    auto segment5 = topology.segment({ vertex3, vertex1 });
    auto cell1 = topology.triangle({ segment3, segment4, segment5 });

    auto segment6 = topology.segment({ vertex2, vertex4 });
    auto segment7 = topology.segment({ vertex4, vertex3 });
    auto segment8 = topology.segment({ vertex3, vertex2 });
    auto cell2 = topology.triangle({ segment6, segment7, segment8 });

    auto segment9 = topology.segment({ vertex4, vertex0 });
    auto segment10 = topology.segment({ vertex0, vertex3 });
    auto segment11 = topology.segment({ vertex3, vertex4 });
    auto cell3 = topology.triangle({ segment9, segment10, segment11 });

    mesh.insert(cell0);
    mesh.insert(cell1);
    mesh.insert(cell2);
    mesh.insert(cell3);

    return;
}


TEST(MetisPartitionerMPI, Base)
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

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // populate the mesh
    build_mesh(geometry, coord_system, mesh);

    // number of partitions
    int n_partitions = simulation.context().n_tasks();

    // rank of the mesh to return
    int n_rank = simulation.context().task_id();

    // partition the mesh
    auto mesh_partition = mito::mesh::metis::partition(mesh, coord_system, n_partitions, n_rank);

    // expect that the mesh was partitioned equally
    // (this check assumes that the number of cells of the original mesh is divisible by the number
    //  of partitions requested)
    EXPECT_EQ(mesh_partition.nCells(), mesh.nCells() / n_partitions);

    // all done
    return;
}


// end of file
