#include "../../base.h"
#include "../../mesh.h"

int
main()
{
    // two vertices
    mito::mesh::vertex_t vertex0;
    mito::mesh::vertex_t vertex1;

    // instantiate an oriented segment
    auto & oriented_segment0 = mito::mesh::segment({ &vertex0, &vertex1 });
    // assert there is only one pointer to the simplex footprint
    assert(oriented_segment0.use_count() == 1);

    // instantiate an identical oriented segment
    auto & oriented_segment1 = mito::mesh::segment({ &vertex0, &vertex1 });
    // assert the factory returned the same oriented segment
    assert(&oriented_segment0 == &oriented_segment1);
    // assert there is still only one pointer to the simplex footprint
    assert(oriented_segment0.use_count() == 1);
    // assert that flipping the flipped simplex gives the original simplex
    assert(&oriented_segment1.flip().flip() == &oriented_segment1); // QUESTION: flip-flip creates the opposite simplex

    // instantiate an oriented segment with opposite orientation
    auto & oriented_segment2 = mito::mesh::segment({ &vertex1, &vertex0 });
    // assert there are now two pointers to the simplex footprint
    assert(oriented_segment0.use_count() == 2);
    // assert that flipping the opposite segment gives the original segment
    assert(&oriented_segment2.flip() == &oriented_segment1);

    // all done
    return 0;
}
