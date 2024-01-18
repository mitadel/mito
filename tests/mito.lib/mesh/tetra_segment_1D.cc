#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>


TEST(Tetra, Segment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 1D
    auto & point_cloud = mito::geometry::point_cloud<1>();

    // a 1D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 1D
    auto coord_system = mito::geometry::coordinate_system<1, mito::geometry::EUCLIDEAN>();

    // an empty mesh of simplicial topology in 1D
    auto mesh = mito::mesh::mesh<mito::geometry::segment_t>(geometry);

    // build nodes
    auto node_0 = mito::geometry::node(geometry, coord_system, { 0.0 });
    auto node_1 = mito::geometry::node(geometry, coord_system, { 1.0 });

    // build segment
    auto segment_a = geometry.segment({ node_0, node_1 });

    // insert cells in the mesh
    mesh.insert(segment_a);

    EXPECT_EQ(mesh.nCells(), 1);

    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, coord_system, 3);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}
