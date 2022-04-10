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

    // build a segment connecting {vertex0} and {vertex1}
    auto & segment0 = mito::mesh::segment({ vertex0, vertex1 });
    // assert there is one segment connecting {vertex0} and {vertex1}
    assert(segment0.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment0).use_count() == 1);

    // build a segment connecting {vertex1} and {vertex3}
    auto & segment1 = mito::mesh::segment({ vertex1, vertex3 });
    // assert there is one segment connecting {vertex1} and {vertex3}
    assert(segment1.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment1).use_count() == 1);

    // flip the segment connecting {vertex1} and {vertex3}
    auto & segment1m = segment1.flip();
    // assert there are two segments connecting {vertex1} and {vertex3}
    assert(segment1.use_count() == 2 && segment1m.use_count() == 2);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment1).use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment1m).use_count() == 1);

    // build a segment connecting {vertex3} and {vertex0}
    auto & segment2 = mito::mesh::segment({ vertex3, vertex0 });
    // assert there is one segment connecting {vertex3} and {vertex0}
    assert(segment2.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment2).use_count() == 1);

    // build a segment connecting {vertex1} and {vertex2}
    auto & segment3 = mito::mesh::segment({ vertex1, vertex2 });
    // assert there is one segment connecting {vertex1} and {vertex2}
    assert(segment3.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment3).use_count() == 1);

    // build a segment connecting {vertex2} and {vertex3}
    auto & segment4 = mito::mesh::segment({ vertex2, vertex3 });
    // assert there is one segment connecting {vertex2} and {vertex3}
    assert(segment4.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment4).use_count() == 1);

    // build a triangle connecting {segment0}, {segment1}, and {segment2}
    auto & element0 = mito::mesh::triangle({ segment0, segment1, segment2 });
    // show me
    std::cout << element0 << std::endl;

    // assert there are two {segment0} (one standalone and one as part of {element0})
    assert(segment0.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment0).use_count() == 2);
    // assert there are three {segment1} (two standalone and one as part of {element0})
    assert(segment1.use_count() == 2 && segment1m.use_count() == 2);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment1).use_count() == 2);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment1m).use_count() == 1);
    // assert there are two {segment2} (one standalone and one as part of {element0})
    assert(segment2.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<1>::Find(segment2).use_count() == 2);

    // assert there is one triangle connecting {segment0}, {segment1}, and {segment2}
    assert(element0.use_count() == 1);
    assert(mito::mesh::OrientedSimplexFactory<2>::Find(element0).use_count() == 1);

    // erase standalone {segment0}
    mito::mesh::OrientedSimplexFactory<1>::Erase(segment0);
    // assert there is one {segment0} (only as part of {element0})
    //assert(segment0.use_count() == 1);

    // erase standalone {segment1}
    mito::mesh::OrientedSimplexFactory<1>::Erase(segment1);
    // assert there are two {segment1} (one standalone and one as part of {element0})
    //assert(segment1.use_count() == 2);

    // erase standalone {segment2}
    mito::mesh::OrientedSimplexFactory<1>::Erase(segment2);
    // assert there is one {segment2} (only as part of {element0})
    //assert(segment2.use_count() == 1);

    // erase {element0}
    // mito::mesh::OrientedSimplexFactory<2>::Erase(element0);
    // assert there is one {segment1m} (one standalone)
    // assert(segment1m.use_count() == 1);

    // build a triangle connecting {segment3}, {segment4}, and {segment1m}
    auto & element1 = mito::mesh::triangle({ segment3, segment4, segment1m });
    // show me
    std::cout << element1 << std::endl;

    // assert there are two {segment3} (one standalone and one as part of {element1})
    //assert(segment3.use_count() == 2);
    // assert there are two {segment4} (one standalone and one as part of {element1})
    //assert(segment4.use_count() == 2);
    // assert there are two {segment1m} (one standalone and one as part of {element1})
    //assert(segment1m.use_count() == 2);

    // assert there is one triangle connecting {segment0}, {segment1}, and {segment2}
    //assert(element1.use_count() == 1);

    // erase standalone {segment3}
    mito::mesh::OrientedSimplexFactory<1>::Erase(segment3);
    // erase standalone {segment4}
    mito::mesh::OrientedSimplexFactory<1>::Erase(segment4);
    // erase standalone {segment1m}
    mito::mesh::OrientedSimplexFactory<1>::Erase(segment1m);

    // erase {element0}
    //mito::mesh::OrientedSimplexFactory<2>::Erase(element1);

#if 0
    mito::mesh::simplex_vector_t<triangle_t> elements = { &element0, &element1, &element2, &element3 };

    // sanity check
    for (const auto & e : elements) {
        if (!e->sanityCheck()) {
            std::cout << "Failed sanity check for element " << e << std::endl;
        }
    }

    // instantiate a manifold as a collection of simplices and a point cloud
    auto manifold = mito::manifolds::manifold(elements, points);
#endif
    // all done
    return 0;
}
