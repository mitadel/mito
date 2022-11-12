#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>
#include <mito/manifolds.h>

using mito::topology::segment_t;
using mito::topology::triangle_t;

TEST(Simplices, TestSimplices)
{
    // an empty topology
    auto topology = mito::topology::topology();

    /**
     * Mesh with four elements:
        (0,1)           (1,1)
          4       6         2
          +-----------------+
          | .             . |
          |   .5       4.   |
          |     .   .       |
         7|       .3    1  3|
          |     .     .     |
          |   .2      1 .   |
          | .      0      . |
          +-----------------+
          0       0        1
        (0,0)             (1,0)
    */

    auto vertex0 = topology.vertex();
    auto vertex1 = topology.vertex();
    auto vertex2 = topology.vertex();
    auto vertex3 = topology.vertex();

    // build a segment connecting {vertex0} and {vertex1}
    auto segment0 = topology.segment({ vertex0, vertex1 });
    // assert there is only one segment connecting {vertex0} and {vertex1}
    EXPECT_FALSE(topology.exists_flipped(segment0));
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment0), 1);

    // build a segment connecting {vertex1} and {vertex3}
    auto segment1 = topology.segment({ vertex1, vertex3 });
    // assert there is only one segment connecting {vertex1} and {vertex3}
    EXPECT_FALSE(topology.exists_flipped(segment1));
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment1), 1);

    // build the flipped segment connecting {vertex1} and {vertex3}
    auto segment1m = topology.flip(segment1);
    // assert there are now two segments connecting {vertex1} and {vertex3}
    EXPECT_TRUE(topology.exists_flipped(segment1));
    EXPECT_TRUE(topology.exists_flipped(segment1m));
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment1), 1);
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment1m), 1);

    // build a segment connecting {vertex3} and {vertex0}
    auto segment2 = topology.segment({ vertex3, vertex0 });
    // assert there is only one segment connecting {vertex3} and {vertex0}
    EXPECT_FALSE(topology.exists_flipped(segment2));
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment2), 1);

    // build a segment connecting {vertex1} and {vertex2}
    auto segment3 = topology.segment({ vertex1, vertex2 });
    // assert there is only one segment connecting {vertex1} and {vertex2}
    EXPECT_FALSE(topology.exists_flipped(segment3));
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment3), 1);

    // build a segment connecting {vertex2} and {vertex3}
    auto segment4 = topology.segment({ vertex2, vertex3 });
    // assert there is only one segment connecting {vertex2} and {vertex3}
    EXPECT_FALSE(topology.exists_flipped(segment4));
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment4), 1);

    // build a triangle connecting {segment0}, {segment1}, and {segment2}
    auto element0 = topology.triangle({ segment0, segment1, segment2 });
    // show me
    std::cout << element0 << std::endl;

    // assert there is one simplex riding on this segment (element0)
    EXPECT_EQ(topology.incidence(segment0), 2);
    // assert there is one simplex riding on this segment (element0)
    EXPECT_EQ(topology.incidence(segment1), 2);
    // assert no simplex rides on this segment yet
    EXPECT_EQ(topology.incidence(segment1m), 1);
    // assert there is one simplex riding on this segment (element0)
    EXPECT_EQ(topology.incidence(segment2), 2);

    // assert there is one triangle connecting {segment0}, {segment1}, and {segment2}
    EXPECT_FALSE(topology.exists_flipped(element0));
    // assert no simplex rides on this segment
    EXPECT_EQ(topology.incidence(element0), 1);

    // build a triangle connecting {segment3}, {segment4}, and {segment1m}
    auto element1 = topology.triangle({ segment3, segment4, segment1m });
    // show me
    std::cout << element1 << std::endl;

    // assert there is one simplex riding on this segment (element1)
    EXPECT_EQ(topology.incidence(segment3), 2);
    // assert there is one simplex riding on this segment (element1)
    EXPECT_EQ(topology.incidence(segment4), 2);
    // assert there is one simplex riding on this segment (element1)
    EXPECT_EQ(topology.incidence(segment1m), 2);

    // assert there is one triangle connecting {segment3}, {segment4}, and {segment1m}
    EXPECT_FALSE(topology.exists_flipped(element1));
    // assert no simplex rides on this segment
    EXPECT_EQ(topology.incidence(element1), 1);

    mito::topology::element_vector_t<triangle_t> elements = { element0, element1 };

    // sanity check
    for (const auto & e : elements) {
        if (!e->sanityCheck()) {
            std::cout << "Failed sanity check for element " << e << std::endl;
        }
    }
}
