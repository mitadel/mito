// code guard
#if !defined(mito_mesh_summit_h)
#define mito_mesh_summit_h

#include <fstream>

namespace mito::mesh {
    template <int D>
    Mesh<D> summit(std::ifstream & fileStream)
    {
        std::cout << "Loading summit mesh..." << std::endl;
        assert(fileStream.is_open());

        // read dimension of physical space
        int dim = 0;
        fileStream >> dim;

        // assert this mesh object is of same dimension of the mesh being read
        assert(int(D) == dim);

        // instantiate mesh
        Mesh<D> mesh;

        // read number of vertices
        int N_vertices = 0;
        fileStream >> N_vertices;

        // read number of elements
        int N_elements = 0;
        fileStream >> N_elements;

        // read number of element types
        int N_element_types = 0;
        fileStream >> N_element_types;

        // QUESTION: Not sure that we need this...
        assert(N_element_types == 1);

        // // read the vertices
        readVertices<D>(fileStream, mesh, N_vertices);

        // // read the elements
        readElements(fileStream, mesh, N_elements);

        // sanity check: the number of vertices in the map is N_vertices
        assert(mesh.template nElements<0>() == N_vertices);

        // sanity check: the number of elements of highest dimension in the map is N_elements
        assert(mesh.template nElements<D>() == N_elements);

        // sanity check: run sanity check for all mesh simplices in cascade
        assert(mesh.sanityCheck());

        // all done
        return mesh;
    }

    template <int D>
    void readVertices(std::ifstream & fileStream, Mesh<D> & mesh, int N_vertices)
    {
        auto & vertices = mesh.template elements<0>();
        // reserve space to read new vertices
        vertices.reserve(vertices.size() + N_vertices);
        // fill in vertices
        for (int n = 0; n < N_vertices; ++n) {
            // instantiate new point
            point_t<D> point;
            for (int d = 0; d < D; ++d) {
                // read point coordinates
                fileStream >> point[d];
            }
            mesh.addVertex(std::move(point));
        }

        // all done
        return;
    }

    template <int D>
    void readElements(std::ifstream & fileStream, Mesh<D> & mesh, int N_elements)
    {
        for (int i = 0; i < N_elements; ++i) {
            int element_type = 0;
            fileStream >> element_type;

            if (element_type == 3) {
                readTriangle(fileStream, mesh);
            } else {
                std::cout << "Error: Unknown element type" << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D>
    void readTriangle(std::ifstream & fileStream, Mesh<D> & mesh)
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

        auto vertex0 = mesh.getVertex(index0);
        auto vertex1 = mesh.getVertex(index1);
        auto vertex2 = mesh.getVertex(index2);

        auto segment0 = segment({ vertex0, vertex1 });
        mesh.addSimplex(segment0);
        auto segment1 = segment({ vertex1, vertex2 });
        mesh.addSimplex(segment1);
        auto segment2 = segment({ vertex2, vertex0 });
        mesh.addSimplex(segment2);

        auto element = triangle({ segment0, segment1, segment2 });
        mesh.addSimplex(element);

        // QUESTION: Can the label be more than one?
        // read label for element
        // TOFIX: Ignored for now
        std::string element_label;
        fileStream >> element_label;

        // all done
        return;
    }
}    // namespace mito::mesh

#endif    // mito_mesh_summit_h