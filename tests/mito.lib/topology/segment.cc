// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/topology.h>


TEST(Topology, Segment)
{
    // build nodes
    auto vertex0 = mito::topology::vertex();
    auto vertex1 = mito::topology::vertex();

    // build segment with a combination of vertices
    auto seg = mito::topology::segment({ vertex0, vertex1 });
    // check that the vertices of the segment are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(seg->vertices(), { vertex0, vertex1 }), +1);

    // build segment with a combination of vertices
    seg = mito::topology::segment({ vertex1, vertex0 });
    // check that the vertices of the segment are a positive permutation of the combination
    // that was requested
    EXPECT_EQ(mito::math::permutation_sign(seg->vertices(), { vertex1, vertex0 }), +1);
}


// end of file
