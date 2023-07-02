#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(FlipDiagonal, TestFlipDiagonal)
{
    // an empty topology
    auto & topology = mito::topology::topology();

    // build vertices
    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();
    auto vertex3 = topology.vertex();

    // build segments
    auto segment_a = topology.segment({ vertex0, vertex1 });
    auto segment_b = topology.segment({ vertex1, vertex2 });
    auto segment_c = topology.segment({ vertex2, vertex3 });
    auto segment_d = topology.segment({ vertex3, vertex0 });
    auto segment_e = topology.segment({ vertex0, vertex2 });
    auto segment_e_flip = topology.flip(segment_e);

    // build triangles
    auto simplex0 = topology.triangle({ segment_a, segment_b, segment_e_flip });
    auto simplex1 = topology.triangle({ segment_e, segment_c, segment_d });

    // build the two triangles obtained by flipping the common edge of the two triangles
    mito::mesh::flipDiagonal(std::make_pair(simplex0, simplex1));

    // assert that new flipped segments have been created
    EXPECT_TRUE(topology.exists({ vertex1, vertex3 }));
    EXPECT_TRUE(topology.exists({ vertex3, vertex1 }));

    // assert that the new diagonal is now in use (by factory, driver, and two triangles)
    auto segment_f = topology.segment({ vertex1, vertex3 });
    EXPECT_EQ(segment_f->footprint().references(), 3);
}
