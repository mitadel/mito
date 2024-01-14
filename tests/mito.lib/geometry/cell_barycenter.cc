#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Barycenter, Segment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // create two nodes
    auto vertex0 = geometry.node({ 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0 });

    // create a segment
    auto segment = topology.segment({ vertex0, vertex1 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(segment, geometry);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::coordinates({ 0.5, 0.0 }));

    // all done
    return;
}

TEST(Barycenter, Triangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<2>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // create three nodes
    auto vertex0 = geometry.node({ 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0 });
    auto vertex2 = geometry.node({ 0.5, 0.5 });

    // create a triangle
    auto triangle = topology.triangle({ vertex0, vertex1, vertex2 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(triangle, geometry);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::coordinates({ 0.5, 1.0 / 6.0 }));

    // all done
    return;
}

TEST(Barycenter, Tetrahedron)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // an empty cloud of points
    auto & point_cloud = mito::geometry::point_cloud<3>();

    // a geometry binding the topology {topology} to the cloud of points {point_cloud}
    auto & geometry = mito::geometry::geometry(topology, point_cloud);

    // create three nodes
    auto vertex0 = geometry.node({ 0.0, 0.0, 0.0 });
    auto vertex1 = geometry.node({ 1.0, 0.0, 0.0 });
    auto vertex2 = geometry.node({ 0.0, 1.0, 0.0 });
    auto vertex3 = geometry.node({ 0.0, 0.0, 1.0 });

    // create a tetrahedron
    auto tetrahedron = topology.tetrahedron({ vertex0, vertex1, vertex2, vertex3 });

    // compute the barycenter position
    auto barycenter = mito::geometry::barycenter(tetrahedron, geometry);

    // check that the barycenter position is correct
    EXPECT_TRUE(barycenter == mito::geometry::coordinates({ 0.25, 0.25, 0.25 }));

    // all done
    return;
}


// end of file
