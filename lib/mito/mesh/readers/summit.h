// code guard
#if !defined(mito_mesh_summit_h)
#define mito_mesh_summit_h

#include <fstream>

namespace mito::mesh {
    template <int D, template <int> class elementT>
    auto readVertices(
        std::ifstream & fileStream, mesh_t<D, elementT> & mesh, int N_vertices,
        std::vector<vertex_t> & vertices, topology_t & topology, point_cloud_t<D> & point_cloud)
        -> void
    {
        // fill in vertices
        for (int n = 0; n < N_vertices; ++n) {
            // instantiate new point
            vector_t<D> coordinates;
            for (int d = 0; d < D; ++d) {
                // read point coordinates
                fileStream >> coordinates[d];
            }

            // instantiate a new vertex
            auto vertex = topology.vertex();

            // instantiate a new point with coordinates {coordinates}
            auto point = point_cloud.point(std::move(coordinates));

            // register vertex-point mapping in {mesh}
            mesh.addVertex(vertex, point);

            // add vertex to {vertices}
            vertices.push_back(vertex);
        }

        // all done
        return;
    }

    template <int D, template <int> class elementT>
    auto readTriangle(
        std::ifstream & fileStream, mesh_t<D, elementT> & mesh,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void
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

        auto segment0 = topology.segment({ vertex0, vertex1 });
        auto segment1 = topology.segment({ vertex1, vertex2 });
        auto segment2 = topology.segment({ vertex2, vertex0 });

        auto element = topology.triangle({ segment0, segment1, segment2 });
        mesh.addSimplex(element);

        // QUESTION: Can the label be more than one?
        // read label for element
        // TOFIX: Ignored for now
        std::string element_label;
        fileStream >> element_label;

        // all done
        return;
    }

    template <int D, template <int> class elementT>
    auto readTetrahedron(
        std::ifstream & fileStream, mesh_t<D, elementT> & mesh,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void
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

        int index3 = 0;
        fileStream >> index3;
        --index3;

        auto vertex0 = vertices[index0];
        auto vertex1 = vertices[index1];
        auto vertex2 = vertices[index2];
        auto vertex3 = vertices[index3];

        auto segment0 = topology.segment({ vertex0, vertex1 });
        auto segment1 = topology.segment({ vertex1, vertex3 });
        auto segment2 = topology.segment({ vertex3, vertex0 });
        auto triangle0 = topology.triangle({ segment0, segment1, segment2 });

        auto segment3 = topology.segment({ vertex1, vertex2 });
        auto segment4 = topology.segment({ vertex2, vertex3 });
        auto segment5 = topology.segment({ vertex3, vertex1 });
        auto triangle1 = topology.triangle({ segment3, segment4, segment5 });

        auto segment6 = topology.segment({ vertex2, vertex0 });
        auto segment7 = topology.segment({ vertex0, vertex3 });
        auto segment8 = topology.segment({ vertex3, vertex2 });
        auto triangle2 = topology.triangle({ segment6, segment7, segment8 });

        auto segment9 = topology.segment({ vertex0, vertex2 });
        auto segment10 = topology.segment({ vertex2, vertex1 });
        auto segment11 = topology.segment({ vertex1, vertex0 });
        auto triangle3 = topology.triangle({ segment9, segment10, segment11 });

        // QUESTION: Can the label be more than one?
        // read label for element
        // TOFIX: Ignored for now
        std::string element_set_id;
        fileStream >> element_set_id;

        auto element = topology.tetrahedron({ triangle0, triangle1, triangle2, triangle3 });
        mesh.addSimplex(element);

        // all done
        return;
    }

    template <int D, template <int> class elementT>
    auto readElements(
        std::ifstream & fileStream, mesh_t<D, elementT> & mesh, int N_elements,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void;

    template <>
    auto readElements(
        std::ifstream & fileStream, mesh_t<2, topology::simplex_t> & mesh, int N_elements,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void
    {
        for (int i = 0; i < N_elements; ++i) {
            int element_type = 0;
            fileStream >> element_type;

            // TODO: add read segment
            if (element_type == 3) {
                readTriangle(fileStream, mesh, vertices, topology);
            } else {
                std::cout << "Error: Unknown element type" << std::endl;
            }
        }

        // all done
        return;
    }

    template <>
    auto readElements(
        std::ifstream & fileStream, mesh_t<3, topology::simplex_t> & mesh, int N_elements,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void
    {
        for (int i = 0; i < N_elements; ++i) {
            int element_type = 0;
            fileStream >> element_type;

            // TODO: add read segment
            if (element_type == 3) {
                readTriangle(fileStream, mesh, vertices, topology);
            } else if (element_type == 4) {
                readTetrahedron(fileStream, mesh, vertices, topology);
            } else {
                std::cout << "Error: Unknown element type" << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D, template <int> class elementT>
    auto summit(std::ifstream & fileStream, topology_t & topology, point_cloud_t<D> & point_cloud)
        -> auto
    {
        std::cout << "Loading summit mesh..." << std::endl;
        assert(fileStream.is_open());

        // read dimension of physical space
        int dim = 0;
        fileStream >> dim;

        // assert this mesh object is of same dimension of the mesh being read
        assert(int(D) == dim);

        // instantiate mesh
        auto mesh = mito::mesh::mesh<D, elementT>();

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

        // read the vertices
        readVertices<D>(fileStream, mesh, N_vertices, vertices, topology, point_cloud);

        // read the elements
        readElements(fileStream, mesh, N_elements, vertices, topology);

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