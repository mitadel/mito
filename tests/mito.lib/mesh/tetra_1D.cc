#include <gtest/gtest.h>
#include <mito/mito.h>


TEST(Tetra, Segment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 1D
    auto & point_cloud = mito::geometry::point_cloud<1>();

    // a 1D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::topology::segment_t>(geometry);

    // build nodes
    auto vertex0 = geometry.node({ 0.0 });
    auto vertex1 = geometry.node({ 1.0 });

    // build segment
    auto segment_a = topology.segment({ vertex0, vertex1 });

    // insert cells in the mesh
    mesh.insert(segment_a);

    EXPECT_EQ(mesh.nCells(), 1);

    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, 3);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}
