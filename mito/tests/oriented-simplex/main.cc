#include "../../base.h"
#include "../../mesh.h"

int
main()
{
    {
        // two vertices
        mito::mesh::vertex_t vertex0;
        mito::mesh::vertex_t vertex1;

        // instantiate an oriented segment
        auto & oriented_segment0 = mito::mesh::segment({ vertex0, vertex1 });
        // assert there is only one pointer to the simplex footprint
        assert(!oriented_segment0.exists_flipped());

        // instantiate an identical oriented segment
        auto & oriented_segment1 = mito::mesh::segment({ vertex0, vertex1 });
        // assert the factory returned the same oriented segment
        assert(&oriented_segment0 == &oriented_segment1);
        // assert there is still only one pointer to the simplex footprint
        assert(!oriented_segment0.exists_flipped());
        // assert that flipping the flipped simplex gives the original simplex
        assert(&oriented_segment1.flip().flip() == &oriented_segment1);

        // instantiate an oriented segment with opposite orientation
        auto & oriented_segment2 = mito::mesh::segment({ vertex1, vertex0 });
        // assert there are now two pointers to the simplex footprint
        assert(oriented_segment0.exists_flipped());
        // assert that flipping the opposite segment gives the original segment
        assert(&oriented_segment2.flip() == &oriented_segment1);
    }
    {
        // build vertices
        mito::mesh::vertex_t vertex0;
        mito::mesh::vertex_t vertex1;
        mito::mesh::vertex_t vertex2;

        // build segments
        auto & segment_a = mito::mesh::segment({ vertex0, vertex1 });
        auto & segment_b = mito::mesh::segment({ vertex1, vertex2 });
        auto & segment_c = mito::mesh::segment({ vertex2, vertex0 });

        // ask factory for a triangle with segment_a, segment_b, segment_c
        auto & element_0 = mito::mesh::triangle({ segment_a, segment_b, segment_c });
        // ask factory for the same triangle (with different order of segments)
        auto & element_1 = mito::mesh::triangle({ segment_b, segment_c, segment_a });
        // assert the factory returned the same object
        assert(&element_0 == &element_1);

        // get the flipped segments
        auto & segment_a_flip = segment_a.flip();    // vertex1 -> vertex0
        auto & segment_b_flip = segment_b.flip();    // vertex2 -> vertex1
        auto & segment_c_flip = segment_c.flip();    // vertex0 -> vertex2

        // ask factory for a triangle with the flipped segments
        auto & element_2 = mito::mesh::triangle({ segment_a_flip, segment_c_flip, segment_b_flip });
        assert(&element_0 != &element_2);

        // get the flipped triangle
        auto & element_0_flip = element_0.flip();
        // assert that the triangle with flipped segments is the same object as the flipped triangle
        assert(&element_0_flip == &element_2);
    }
    // all done
    return 0;
}
