#include <gtest/gtest.h>
#include <mito/mito.h>

using edge_t = mito::topology::segment_t;


TEST(CellEdges, Triangle)
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

    // get the (oriented) edges of the triangle
    std::set<edge_t> edges;
    triangle->edges(edges);

    // build the (oriented) edges based on the vertices
    std::set<edge_t> segments;
    segments.insert(topology.segment({ vertex0, vertex1 }));
    segments.insert(topology.segment({ vertex1, vertex2 }));
    segments.insert(topology.segment({ vertex2, vertex0 }));

    // check that the edges fetched correspond with the segments connecting the vertices
    EXPECT_TRUE(edges == segments);

    // all done
    return;
}

TEST(CellEdges, Tetrahedron)
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

    // get the edges of the tetrahedron
    std::set<edge_t> edges;
    tetrahedron->edges(edges);

    // build the (oriented) edges based on the vertices
    std::set<edge_t> segments;
    segments.insert(topology.segment({ vertex0, vertex1 }));
    segments.insert(topology.segment({ vertex1, vertex3 }));
    segments.insert(topology.segment({ vertex3, vertex0 }));
    segments.insert(topology.segment({ vertex1, vertex2 }));
    segments.insert(topology.segment({ vertex2, vertex0 }));
    segments.insert(topology.segment({ vertex2, vertex3 }));
    segments.insert(topology.segment({ vertex3, vertex1 }));
    segments.insert(topology.segment({ vertex0, vertex3 }));
    segments.insert(topology.segment({ vertex3, vertex2 }));
    segments.insert(topology.segment({ vertex0, vertex2 }));
    segments.insert(topology.segment({ vertex2, vertex1 }));
    segments.insert(topology.segment({ vertex1, vertex0 }));

    // check that the edges fetched correspond with the segments connecting the vertices
    EXPECT_TRUE(edges == segments);

    // all done
    return;
}


// end of file
