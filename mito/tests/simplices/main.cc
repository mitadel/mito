#include <map>
#include "../../mesh/Simplex.h"
#include "../../mesh/ElementSet.h"

using mito::point_t;
using mito::vertex_t;
using mito::segment_t;
using mito::triangle_t;

int
main()
{

    mito::VertexCoordinatesMap<2> vertexCoordinatesMap;

    /**
     * Mesh with four elements:
        (0,1)           (1,1)
          4       6         2
          +-----------------+
          | .             . |
          |   .5       4.   |
          |     .   .       |
         7|       .3       3|
          |     .     .     |
          |   .2      1 .   |
          | .             . |
          +-----------------+
          0       0        1
        (0,0)             (1,0)
    */

    vertex_t vertex0;
    vertexCoordinatesMap.insert(vertex0, point_t<2> { 0.0, 0.0 });
    vertex_t vertex1;
    vertexCoordinatesMap.insert(vertex1, point_t<2> { 1.0, 0.0 });
    vertex_t vertex2;
    vertexCoordinatesMap.insert(vertex2, point_t<2> { 1.0, 1.0 });
    vertex_t vertex3;
    vertexCoordinatesMap.insert(vertex3, point_t<2> { 0.5, 0.5 });
    vertex_t vertex4;
    vertexCoordinatesMap.insert(vertex4, point_t<2> { 0.0, 1.0 });

    segment_t segment0({ &vertex0, &vertex1 });
    segment_t segment1({ &vertex1, &vertex3 });
    segment_t segment2({ &vertex3, &vertex0 });
    segment_t segment3({ &vertex1, &vertex2 });
    segment_t segment4({ &vertex2, &vertex3 });
    segment_t segment5({ &vertex4, &vertex3 });
    segment_t segment6({ &vertex2, &vertex4 });
    segment_t segment7({ &vertex4, &vertex0 });

    triangle_t element0({ &segment0, &segment1, &segment2 });
    triangle_t element1({ &segment3, &segment4, &segment1 });
    triangle_t element2({ &segment6, &segment5, &segment4 });
    triangle_t element3({ &segment7, &segment2, &segment5 });

    std::vector<triangle_t *> elements = { &element0, &element1, &element2, &element3 };

    // sanity check
    for (const auto & e : elements) {
        if (!e->sanityCheck()) {
            std::cout << "Failed sanity check for element " << e << std::endl;
        }
    }

    // instantiate an ElementSet as a collection of simplices and a vertex-coordinates mapping.
    mito::ElementSet elementSet(elements, vertexCoordinatesMap);

    // all done
    return 0;
}
