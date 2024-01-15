#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Barycenter, Tetrahedron)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // a Euclidean coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::EUCLIDEAN>();

    // create four nodes
    auto vertex0 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0, 0.0 });
    auto vertex1 = mito::geometry::node(geometry, coord_system, { 1.0, 0.0, 0.0 });
    auto vertex2 = mito::geometry::node(geometry, coord_system, { 0.0, 1.0, 0.0 });
    auto vertex3 = mito::geometry::node(geometry, coord_system, { 0.0, 0.0, 1.0 });

    // create a tetrahedron
    auto tetrahedron = topology.tetrahedron({ vertex0, vertex1, vertex2, vertex3 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(tetrahedron, geometry, coord_system);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::coordinates({ 0.25, 0.25, 0.25 }));

    // all done
    return;
}


// end of file
