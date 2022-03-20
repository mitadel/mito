#include "../../base.h"
#include "../../mesh.h"

using mito::mesh::point_t;
using mito::mesh::vertex_t;
using mito::mesh::segment_t;
using mito::mesh::triangle_t;

int
main()
{

    mito::mesh::VertexSet<2> vertexCoordinatesMap;

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

    auto & segment0 = mito::mesh::segment({ &vertex0, &vertex1 });
    auto & segment1 = mito::mesh::segment({ &vertex1, &vertex3 });
    auto & segment1m = mito::mesh::segment({ &vertex3, &vertex1 });
    auto & segment2 = mito::mesh::segment({ &vertex3, &vertex0 });
    auto & segment2m = mito::mesh::segment({ &vertex0, &vertex3 });
    auto & segment3 = mito::mesh::segment({ &vertex1, &vertex2 });
    auto & segment4 = mito::mesh::segment({ &vertex2, &vertex3 });
    auto & segment4m = mito::mesh::segment({ &vertex3, &vertex2 });
    auto & segment5 = mito::mesh::segment({ &vertex4, &vertex3 });
    auto & segment5m = mito::mesh::segment({ &vertex3, &vertex4 });
    auto & segment6 = mito::mesh::segment({ &vertex2, &vertex4 });
    auto & segment7 = mito::mesh::segment({ &vertex4, &vertex0 });

    auto & element0 = mito::mesh::triangle({ &segment0, &segment1, &segment2 });
    auto & element1 = mito::mesh::triangle({ &segment3, &segment4, &segment1m });
    auto & element2 = mito::mesh::triangle({ &segment6, &segment5, &segment4m });
    auto & element3 = mito::mesh::triangle({ &segment7, &segment2m, &segment5 });

    std::vector<triangle_t *> elements = { &element0, &element1, &element2, &element3 };

    // sanity check
    for (const auto & e : elements) {
        if (!e->sanityCheck()) {
            std::cout << "Failed sanity check for element " << e << std::endl;
        }
    }

    // instantiate an ElementSet as a collection of simplices and a vertex-coordinates mapping.
    mito::mesh::ElementSet elementSet(elements, vertexCoordinatesMap);
    
    // all done
    return 0;
}
