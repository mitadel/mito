#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>


TEST(Tetra, Triangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points in 2D
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // an empty mesh of triangles
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t>(geometry);

    // build nodes of a triangle (counterclockwise order)
    auto node_1 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0 });
    auto node_2 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0 });
    auto node_3 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0 });

    // build triangle with a positive volume
    auto triangle = geometry.triangle({ node_1, node_2, node_3 });

    // insert triangle in mesh
    mesh.insert(triangle);

    // do tetra mesh refinements
    auto tetra_mesh = mito::mesh::tetra(mesh, geometry, coord_system, 2);
}
