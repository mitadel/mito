// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/topology.h>


TEST(FlipDiagonal, TestFlipDiagonal)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build vertices
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();
    auto vertex3 = topology.vertex();

    // build triangles
    auto simplex0 = topology.triangle({ vertex0, vertex1, vertex2 });
    auto simplex1 = topology.triangle({ vertex0, vertex2, vertex3 });

    // build the two triangles obtained by flipping the common edge of the two triangles
    [[maybe_unused]] auto simplex_pair =
        mito::topology::flipDiagonal(std::make_pair(simplex0, simplex1));

    // assert that new flipped segments have been created
    EXPECT_TRUE(topology.exists({ vertex1, vertex3 }));
    EXPECT_TRUE(topology.exists({ vertex3, vertex1 }));

    // assert that the new diagonal is now in use (by factory, driver, and two triangles)
    auto segment_f = topology.segment({ vertex1, vertex3 });
    EXPECT_EQ(segment_f->footprint().references(), 2);

    // erase the old simplices
    topology.erase(simplex0);
    topology.erase(simplex1);
    // assert that old diagonals have been erased
    EXPECT_FALSE(topology.exists({ vertex0, vertex2 }));
    EXPECT_FALSE(topology.exists({ vertex2, vertex0 }));
}
