// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/topology.h>

using edge_t = mito::topology::segment_t;


TEST(CellEdges, Triangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // create three vertices
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();

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

    // create four vertices
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();
    auto vertex3 = topology.vertex();

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
