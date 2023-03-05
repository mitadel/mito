#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(Tetra, MeshTwoTriangles)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // an empty mesh of simplicial topology in 2D
    auto mesh = mito::mesh::mesh<mito::topology::triangle_t>(geometry);

    // build nodes
    auto & vertex0 = geometry.node({ 0.0, 0.0 });
    auto & vertex1 = geometry.node({ 1.0, 0.0 });
    auto & vertex2 = geometry.node({ 1.0, 1.0 });
    auto & vertex3 = geometry.node({ 0.0, 1.0 });

    // build segments
    auto & segment_a = topology.segment({ vertex0, vertex1 });
    auto & segment_b = topology.segment({ vertex1, vertex2 });
    auto & segment_c = topology.segment({ vertex2, vertex3 });
    auto & segment_d = topology.segment({ vertex3, vertex0 });
    auto & segment_e = topology.segment({ vertex0, vertex2 });
    auto & segment_e_flip = topology.flip(segment_e);

    // build triangles
    auto & simplex0 = topology.triangle({ segment_a, segment_b, segment_e_flip });
    auto & simplex1 = topology.triangle({ segment_e, segment_c, segment_d });

    // insert cells in the mesh
    mesh.insert(simplex0);
    mesh.insert(simplex1);

    EXPECT_EQ(mesh.nCells(), 2);

    auto tetra_mesh = mito::mesh::tetra(mesh, geometry);

    EXPECT_EQ(tetra_mesh.nCells(), 8);
}

TEST(Tetra, MeshRectangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a 2D geometry binding the topology {topology} on the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    std::ifstream fileStream("rectangle.summit");
    auto mesh = mito::mesh::summit<mito::topology::triangle_t>(fileStream, geometry);

    int cells = mesh.nCells();

    auto tetra_mesh = tetra(mesh, geometry);

    EXPECT_EQ(tetra_mesh.nCells(), 4 * cells);
}