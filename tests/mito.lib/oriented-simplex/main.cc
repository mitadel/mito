#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/mesh.h>

TEST(OrientedSimplex, TestOrientedSimplex)
{
    {
        // two vertices
        auto vertex0 = mito::topology::vertex();
        auto vertex1 = mito::topology::vertex();

        // instantiate an oriented segment
        auto oriented_segment0 = mito::topology::segment({ vertex0, vertex1 });
        // assert there is only one pointer to the simplex footprint
        assert(!mito::topology::exists_flipped(oriented_segment0));

        // instantiate an identical oriented segment
        auto oriented_segment1 = mito::topology::segment({ vertex0, vertex1 });
        // assert the factory returned the same oriented segment
        assert((*oriented_segment0).simplex_id() == (*oriented_segment1).simplex_id());
        // assert there is still only one pointer to the simplex footprint
        assert(!mito::topology::exists_flipped(oriented_segment0));
        // assert that flipping the flipped simplex gives the original simplex
        assert(
            (*mito::topology::flip(mito::topology::flip(oriented_segment1))).simplex_id()
            == (*oriented_segment1).simplex_id());

        // instantiate an oriented segment with opposite orientation
        auto oriented_segment2 = mito::topology::segment({ vertex1, vertex0 });
        // assert there are now two pointers to the simplex footprint
        assert(mito::topology::exists_flipped(oriented_segment0));
        // assert that flipping the opposite segment gives the original segment
        assert(
            (*mito::topology::flip(oriented_segment2)).simplex_id()
            == (*oriented_segment1).simplex_id());
    }
    {
        // build vertices
        auto vertex0 = mito::topology::vertex();
        auto vertex1 = mito::topology::vertex();
        auto vertex2 = mito::topology::vertex();

        // build segments
        auto segment_a = mito::topology::segment({ vertex0, vertex1 });
        auto segment_b = mito::topology::segment({ vertex1, vertex2 });
        auto segment_c = mito::topology::segment({ vertex2, vertex0 });

        // ask factory for a triangle with segment_a, segment_b, segment_c
        auto element_0 = mito::topology::triangle({ segment_a, segment_b, segment_c });
        // ask factory for the same triangle (with different order of segments)
        auto element_1 = mito::topology::triangle({ segment_b, segment_c, segment_a });
        // assert the factory returned the same object
        assert((*element_0).simplex_id() == (*element_1).simplex_id());

        // get the flipped segments
        auto segment_a_flip = mito::topology::flip(segment_a);    // vertex1 -> vertex0
        auto segment_b_flip = mito::topology::flip(segment_b);    // vertex2 -> vertex1
        auto segment_c_flip = mito::topology::flip(segment_c);    // vertex0 -> vertex2

        // ask factory for a triangle with the flipped segments
        auto element_2 =
            mito::topology::triangle({ segment_a_flip, segment_c_flip, segment_b_flip });
        assert((*element_0).simplex_id() != (*element_2).simplex_id());

        // get the flipped triangle
        auto element_0_flip = mito::topology::flip(element_0);
        // assert that the triangle with flipped segments is the same object as the flipped triangle
        assert((*element_0_flip).simplex_id() == (*element_2).simplex_id());
    }
}
