#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/topology.h>


TEST(Topology, Segment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build nodes
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();

    // build segment with a combination of vertices
    auto seg = topology.segment({ vertex0, vertex1 });
    // check that the vertices of the segment are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(seg->vertices2(), { vertex0, vertex1 }), +1);

    // build segment with a combination of vertices
    seg = topology.segment({ vertex1, vertex0 });
    // check that the vertices of the segment are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(seg->vertices2(), { vertex1, vertex0 }), +1);
}


// end of file