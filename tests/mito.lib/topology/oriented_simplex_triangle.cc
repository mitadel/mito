#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/topology.h>


TEST(OrientedSimplex, Triangle)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build vertices
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();

    // build segments
    auto segment_a = topology.segment({ vertex0, vertex1 });
    auto segment_b = topology.segment({ vertex1, vertex2 });
    auto segment_c = topology.segment({ vertex2, vertex0 });

    // ask factory for a triangle with segment_a, segment_b, segment_c
    auto cell0 = topology.triangle({ segment_a, segment_b, segment_c });
    // ask factory for the same triangle (with different order of segments)
    auto cell1 = topology.triangle({ segment_b, segment_c, segment_a });
    // assert the factory returned the same object
    EXPECT_TRUE(cell0.id() == cell1.id());

    // get the flipped segments
    auto segment_a_flip = topology.flip(segment_a);    // vertex1 -> vertex0
    auto segment_b_flip = topology.flip(segment_b);    // vertex2 -> vertex1
    auto segment_c_flip = topology.flip(segment_c);    // vertex0 -> vertex2

    // ask factory for a triangle with the flipped segments
    auto cell2 = topology.triangle({ segment_a_flip, segment_c_flip, segment_b_flip });
    EXPECT_TRUE(cell0.id() != cell2.id());

    // get the flipped triangle
    auto cell0_flip = topology.flip(cell0);
    // assert that the triangle with flipped segments is the same object as the flipped triangle
    EXPECT_TRUE(cell0_flip.id() == cell2.id());
}
