#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Geometry, GeometricTriangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {cloud}
    auto & geometry = mito::geometry::geometry(topology, cloud);

    // a Euclidean coordinate system in 2D
    auto coord_system = mito::geometry::coordinate_system<2, mito::geometry::EUCLIDEAN>();

    // build nodes
    auto node_0 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0 });
    auto node_1 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0 });
    auto node_2 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0 });

    // build a geometric with a combination of vertices
    auto triangle = geometry.triangle({ node_0, node_1, node_2 });
}
