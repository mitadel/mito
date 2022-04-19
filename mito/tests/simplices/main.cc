#include "../../base.h"
#include "../../mesh.h"
#include "../../manifolds.h"

using mito::mesh::point_t;
using mito::mesh::vertex_t;
using mito::mesh::segment_t;
using mito::mesh::triangle_t;

int
main()
{
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

    // a cloud of points
    auto points = mito::mesh::point_cloud<2>();

    // QUESTION: should we define a point as a pairing between a vertex and its coordinates? 
    vertex_t vertex0;
    points.insert(vertex0, point_t<2> { 0.0, 0.0 });
    vertex_t vertex1;
    points.insert(vertex1, point_t<2> { 1.0, 0.0 });
    vertex_t vertex2;
    points.insert(vertex2, point_t<2> { 1.0, 1.0 });
    vertex_t vertex3;
    points.insert(vertex3, point_t<2> { 0.5, 0.5 });

    // show me
    std::cout << points << std::endl;

    // QUESTION: How about we return the simplices from the factories as reference wrappers? 
    // build a segment connecting {vertex0} and {vertex1}
    auto segment0 = mito::mesh::segment({ vertex0, vertex1 });
    // assert there is only one segment connecting {vertex0} and {vertex1}
    assert(!mito::mesh::exists_flipped(segment0));
    // assert no simplex rides on this segment yet
    assert(segment0->incidence() == 1);

    // build a segment connecting {vertex1} and {vertex3}
    auto segment1 = mito::mesh::segment({ vertex1, vertex3 });
    // assert there is only one segment connecting {vertex1} and {vertex3}
    assert(!mito::mesh::exists_flipped(segment1));
    // assert no simplex rides on this segment yet
    assert(segment1->incidence() == 1);

    // build the flipped segment connecting {vertex1} and {vertex3}
    auto segment1m = mito::mesh::flip(segment1);
    // assert there are now two segments connecting {vertex1} and {vertex3}
    assert(mito::mesh::exists_flipped(segment1) && mito::mesh::exists_flipped(segment1m));
    // assert no simplex rides on this segment yet
    assert(segment1->incidence() == 1);
    // assert no simplex rides on this segment yet
    assert(segment1m->incidence() == 1);

    // build a segment connecting {vertex3} and {vertex0}
    auto segment2 = mito::mesh::segment({ vertex3, vertex0 });
    // assert there is only one segment connecting {vertex3} and {vertex0}
    assert(!mito::mesh::exists_flipped(segment2));
    // assert no simplex rides on this segment yet
    assert(segment2->incidence() == 1);

    // build a segment connecting {vertex1} and {vertex2}
    auto segment3 = mito::mesh::segment({ vertex1, vertex2 });
    // assert there is only one segment connecting {vertex1} and {vertex2}
    assert(!mito::mesh::exists_flipped(segment3));
    // assert no simplex rides on this segment yet
    assert(segment3->incidence() == 1);

    // build a segment connecting {vertex2} and {vertex3}
    auto segment4 = mito::mesh::segment({ vertex2, vertex3 });
    // assert there is only one segment connecting {vertex2} and {vertex3}
    assert(!mito::mesh::exists_flipped(segment4));
    // assert no simplex rides on this segment yet
    assert(segment4->incidence() == 1);

    // build a triangle connecting {segment0}, {segment1}, and {segment2}
    auto element0 = mito::mesh::triangle({ segment0, segment1, segment2 });
    // show me
    std::cout << *element0 << std::endl;

    // assert there is one simplex riding on this segment (element0) 
    assert(segment0->incidence() == 2);
    // assert there is one simplex riding on this segment (element0)
    assert(segment1->incidence() == 2);
    // assert no simplex rides on this segment yet
    assert(segment1m->incidence() == 1);
    // assert there is one simplex riding on this segment (element0)
    assert(segment2->incidence() == 2);

    // assert there is one triangle connecting {segment0}, {segment1}, and {segment2}
    assert(!mito::mesh::exists_flipped(element0));
    // assert no simplex rides on this segment
    assert(element0->incidence() == 1);

    // build a triangle connecting {segment3}, {segment4}, and {segment1m}
    auto element1 = mito::mesh::triangle({ segment3, segment4, segment1m });
    // show me
    std::cout << *element1 << std::endl;

    // assert there is one simplex riding on this segment (element1)
    assert(segment3->incidence() == 2);
    // assert there is one simplex riding on this segment (element1)
    assert(segment4->incidence() == 2);
    // assert there is one simplex riding on this segment (element1)
    assert(segment1m->incidence() == 2);

    // assert there is one triangle connecting {segment3}, {segment4}, and {segment1m}
    assert(!mito::mesh::exists_flipped(element1));
    // assert no simplex rides on this segment
    assert(element1->incidence() == 1);

    mito::mesh::simplex_vector_t<triangle_t> elements = { element0, element1 };

    // sanity check
    for (const auto & e : elements) {
        if (!e->sanityCheck()) {
            std::cout << "Failed sanity check for element " << e << std::endl;
        }
    }

    // all done
    return 0;
}
