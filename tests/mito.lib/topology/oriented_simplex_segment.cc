// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/topology.h>


TEST(OrientedSimplex, Segment)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // two vertices
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();

    // instantiate an oriented segment
    auto oriented_segment0 = topology.segment({ vertex0, vertex1 });
    // assert there is only one pointer to the simplex footprint
    EXPECT_FALSE(topology.exists_flipped(oriented_segment0));

    // instantiate an identical oriented segment
    auto oriented_segment1 = topology.segment({ vertex0, vertex1 });
    // assert the factory returned the same oriented segment
    EXPECT_TRUE(oriented_segment0.id() == oriented_segment1.id());
    // assert there is still only one pointer to the simplex footprint
    EXPECT_FALSE(topology.exists_flipped(oriented_segment0));
    // assert that flipping the flipped simplex gives the original simplex
    EXPECT_TRUE(topology.flip(topology.flip(oriented_segment1)).id() == oriented_segment1.id());

    // instantiate an oriented segment with opposite orientation
    auto oriented_segment2 = topology.segment({ vertex1, vertex0 });
    // assert there are now two pointers to the simplex footprint
    EXPECT_TRUE(topology.exists_flipped(oriented_segment0));
    // assert that flipping the opposite segment gives the original segment
    EXPECT_TRUE(topology.flip(oriented_segment2).id() == oriented_segment1.id());
}
