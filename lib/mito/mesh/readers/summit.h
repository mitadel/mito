// code guard
#if !defined(mito_mesh_summit_h)
#define mito_mesh_summit_h

#include <fstream>

namespace mito::mesh {
    template <int D, template <int> class cellT, int N>
    auto readVertices(
        std::ifstream & fileStream, mesh_t<D, cellT, N> & mesh, int N_vertices,
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
            const auto & vertex = topology.vertex();

            // instantiate a new point with coordinates {coordinates}
            const auto & point = point_cloud.point(std::move(coordinates));

            // register vertex-point mapping in {mesh}
            mesh.insert(vertex, point);

            // add vertex to {vertices}
            vertices.push_back(vertex);
        }

        // all done
        return;
    }

    template <int D, template <int> class cellT, int N>
    auto readTriangle(
        std::ifstream & fileStream, mesh_t<D, cellT, N> & mesh,
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

        const auto & vertex0 = vertices[index0];
        const auto & vertex1 = vertices[index1];
        const auto & vertex2 = vertices[index2];

        const auto & segment0 = topology.segment({ vertex0, vertex1 });
        const auto & segment1 = topology.segment({ vertex1, vertex2 });
        const auto & segment2 = topology.segment({ vertex2, vertex0 });

        const auto & cell = topology.triangle({ segment0, segment1, segment2 });
        mesh.insert(cell);

        // QUESTION: Can the label be more than one?
        // read label for cell
        // TOFIX: Ignored for now
        std::string cell_label;
        fileStream >> cell_label;

        // all done
        return;
    }

    template <int D, template <int> class cellT, int N>
    auto readTetrahedron(
        std::ifstream & fileStream, mesh_t<D, cellT, N> & mesh,
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

        const auto & vertex0 = vertices[index0];
        const auto & vertex1 = vertices[index1];
        const auto & vertex2 = vertices[index2];
        const auto & vertex3 = vertices[index3];

        const auto & segment0 = topology.segment({ vertex0, vertex1 });
        const auto & segment1 = topology.segment({ vertex1, vertex3 });
        const auto & segment2 = topology.segment({ vertex3, vertex0 });
        const auto & triangle0 = topology.triangle({ segment0, segment1, segment2 });

        const auto & segment3 = topology.segment({ vertex1, vertex2 });
        const auto & segment4 = topology.segment({ vertex2, vertex3 });
        const auto & segment5 = topology.segment({ vertex3, vertex1 });
        const auto & triangle1 = topology.triangle({ segment3, segment4, segment5 });

        const auto & segment6 = topology.segment({ vertex2, vertex0 });
        const auto & segment7 = topology.segment({ vertex0, vertex3 });
        const auto & segment8 = topology.segment({ vertex3, vertex2 });
        const auto & triangle2 = topology.triangle({ segment6, segment7, segment8 });

        const auto & segment9 = topology.segment({ vertex0, vertex2 });
        const auto & segment10 = topology.segment({ vertex2, vertex1 });
        const auto & segment11 = topology.segment({ vertex1, vertex0 });
        const auto & triangle3 = topology.triangle({ segment9, segment10, segment11 });

        // QUESTION: Can the label be more than one?
        // read label for cell
        // TOFIX: Ignored for now
        std::string cell_set_id;
        fileStream >> cell_set_id;

        const auto & cell = topology.tetrahedron({ triangle0, triangle1, triangle2, triangle3 });
        mesh.insert(cell);

        // all done
        return;
    }

    template <int D, template <int> class cellT, int N>
    auto readElements(
        std::ifstream & fileStream, mesh_t<D, cellT, N> & mesh, int N_cells,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void;

    template <int N>
    auto readElements(
        std::ifstream & fileStream, mesh_t<2, topology::simplex_t, N> & mesh, int N_cells,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            // TODO: add read segment
            if (cell_type == 3) {
                readTriangle(fileStream, mesh, vertices, topology);
            } else {
                std::cout << "Error: Unknown cell type" << std::endl;
            }
        }

        // all done
        return;
    }

    template <int N>
    auto readElements(
        std::ifstream & fileStream, mesh_t<3, topology::simplex_t, N> & mesh, int N_cells,
        const std::vector<vertex_t> & vertices, topology_t & topology) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            // TODO: add read segment
            if (cell_type == 3) {
                readTriangle(fileStream, mesh, vertices, topology);
            } else if (cell_type == 4) {
                readTetrahedron(fileStream, mesh, vertices, topology);
            } else {
                std::cout << "Error: Unknown cell type" << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D, template <int> class cellT, int N = D>
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
        auto mesh = mito::mesh::mesh<D, cellT, N>();

        // read number of vertices
        int N_vertices = 0;
        fileStream >> N_vertices;
        // reserve space for vertices
        std::vector<vertex_t> vertices;
        vertices.reserve(N_vertices);

        // read number of cells
        int N_cells = 0;
        fileStream >> N_cells;

        // read number of cell types
        int N_cell_types = 0;
        fileStream >> N_cell_types;

        // QUESTION: Not sure that we need this...
        assert(N_cell_types == 1);

        // read the vertices
        readVertices<D>(fileStream, mesh, N_vertices, vertices, topology, point_cloud);

        // read the cells
        readElements(fileStream, mesh, N_cells, vertices, topology);

        // sanity check: the number of vertices in the map is N_vertices
        vertices.shrink_to_fit();
        assert(vertices.size() == static_cast<size_t>(N_vertices));

        // sanity check: the number of cells of highest dimension in the map is N_cells
        assert(mesh.template nCells<D>() == N_cells);

        // sanity check: run sanity check for all mesh simplices in cascade
        assert(mesh.sanityCheck());

        // all done
        return mesh;
    }

}    // namespace mito::mesh

#endif    // mito_mesh_summit_h