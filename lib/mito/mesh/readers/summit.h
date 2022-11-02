// code guard
#if !defined(mito_mesh_summit_h)
#define mito_mesh_summit_h

#include <fstream>

namespace mito::mesh {
    template <int D>
    auto readVertices(std::ifstream & fileStream, int N_vertices, std::vector<vertex_t> & vertices)
        -> void
    {
        // fill in vertices
        for (int n = 0; n < N_vertices; ++n) {
            // instantiate new point
            point_t<D> point;
            for (int d = 0; d < D; ++d) {
                // read point coordinates
                fileStream >> point[d];
            }

            // instantiate new vertex
            auto vertex = geometry::vertex(std::move(point));

            // instantiate new vertex and add it to {vertices}
            vertices.push_back(vertex);
        }

        // all done
        return;
    }

    template <int D>
    auto readElements(
        std::ifstream & fileStream, Mesh<D, simplex_t> & mesh, int N_elements,
        const std::vector<vertex_t> & vertices) -> void
    {
        for (int i = 0; i < N_elements; ++i) {
            int element_type = 0;
            fileStream >> element_type;

            if (element_type == 3) {
                readTriangle(fileStream, mesh, vertices);
            } else {
                std::cout << "Error: Unknown element type" << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D>
    auto readTriangle(
        std::ifstream & fileStream, Mesh<D, simplex_t> & mesh,
        const std::vector<vertex_t> & vertices) -> void
    {
        int index0 = 0;
        fileStream >> index0;
        --index0;

        int index1 = 0;
        fileStream >> index1;
        --index1;

        int index2 = 0;
        fileStream >> index2;
        --index2;

        auto vertex0 = vertices[index0];
        auto vertex1 = vertices[index1];
        auto vertex2 = vertices[index2];

        auto segment0 = mito::topology::segment({ vertex0, vertex1 });
        auto segment1 = mito::topology::segment({ vertex1, vertex2 });
        auto segment2 = mito::topology::segment({ vertex2, vertex0 });

        auto element = mito::topology::triangle({ segment0, segment1, segment2 });
        mesh.addSimplex(element);

        // QUESTION: Can the label be more than one?
        // read label for element
        // TOFIX: Ignored for now
        std::string element_label;
        fileStream >> element_label;

        // all done
        return;
    }

    template <int D>
    auto summit(std::ifstream & fileStream) -> auto
    {
        std::cout << "Loading summit mesh..." << std::endl;
        assert(fileStream.is_open());

        // read dimension of physical space
        int dim = 0;
        fileStream >> dim;

        // assert this mesh object is of same dimension of the mesh being read
        assert(int(D) == dim);

        // instantiate mesh of simplicial topology
        Mesh<D, simplex_t> mesh;

        // read number of vertices
        int N_vertices = 0;
        fileStream >> N_vertices;
        // reserve space for vertices
        std::vector<vertex_t> vertices;
        vertices.reserve(N_vertices);

        // read number of elements
        int N_elements = 0;
        fileStream >> N_elements;

        // read number of element types
        int N_element_types = 0;
        fileStream >> N_element_types;

        // QUESTION: Not sure that we need this...
        assert(N_element_types == 1);

        // QUESTION: do we need to reserve space for elements before reading them?

        // read the vertices
        readVertices<D>(fileStream, N_vertices, vertices);

        // read the elements
        readElements(fileStream, mesh, N_elements, vertices);

        // sanity check: the number of vertices in the map is N_vertices
        vertices.shrink_to_fit();
        assert(vertices.size() == static_cast<size_t>(N_vertices));

        // sanity check: the number of elements of highest dimension in the map is N_elements
        assert(mesh.template nElements<D>() == N_elements);

        // sanity check: run sanity check for all mesh simplices in cascade
        assert(mesh.sanityCheck());

        // all done
        return mesh;
    }

}    // namespace mito::mesh

#endif    // mito_mesh_summit_h