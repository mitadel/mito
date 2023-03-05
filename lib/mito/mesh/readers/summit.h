// code guard
#if !defined(mito_mesh_summit_h)
#define mito_mesh_summit_h

#include <fstream>

namespace mito::mesh {
    template <int D>
    auto readVertices(
        std::ifstream & fileStream, mito::geometry::geometry_t<D> & geometry, int N_vertices,
        std::vector<vertex_t> & vertices) -> void
    {

        // fill in vertices
        for (int n = 0; n < N_vertices; ++n) {
            // instantiate new point
            vector_t<D> coordinates;
            for (int d = 0; d < D; ++d) {
                // read point coordinates
                fileStream >> coordinates[d];
            }

            // instantiate a new node at {coordinates}
            const auto & vertex = geometry.node(std::move(coordinates));

            // add vertex to {vertices}
            vertices.push_back(vertex);
        }

        // all done
        return;
    }

    template <int D>
    auto readSegment(
        std::ifstream & fileStream, mesh_t<topology::segment_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, const std::vector<vertex_t> & vertices) -> void
    {
        auto & topology = geometry.topology();

        int index0 = 0;
        fileStream >> index0;
        --index0;

        int index1 = 0;
        fileStream >> index1;
        --index1;

        const auto & vertex0 = vertices[index0];
        const auto & vertex1 = vertices[index1];

        const auto & segment = topology.segment({ vertex0, vertex1 });
        mesh.insert(segment);

        // QUESTION: Can the label be more than one?
        // read label for cell
        // TOFIX: Ignored for now
        std::string cell_label;
        fileStream >> cell_label;

        // all done
        return;
    }

    template <int D>
    auto readTriangle(
        std::ifstream & fileStream, mesh_t<topology::triangle_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, const std::vector<vertex_t> & vertices) -> void
    {
        auto & topology = geometry.topology();

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

    template <int D>
    auto readTetrahedron(
        std::ifstream & fileStream, mesh_t<topology::tetrahedron_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, const std::vector<vertex_t> & vertices) -> void
    {
        auto & topology = geometry.topology();

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

    template <int D>
    auto readSegments(
        std::ifstream & fileStream, mesh_t<topology::segment_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, int N_cells,
        const std::vector<vertex_t> & vertices) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            // TODO: add read segment
            if (cell_type == 2) {
                readSegment(fileStream, mesh, geometry, vertices);
            } else {
                std::cout << "Not a segment, skipping element..." << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D>
    auto readTriangles(
        std::ifstream & fileStream, mesh_t<topology::triangle_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, int N_cells,
        const std::vector<vertex_t> & vertices) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            // TODO: add read segment
            if (cell_type == 3) {
                readTriangle(fileStream, mesh, geometry, vertices);
            } else {
                std::cout << "Not a triangle, skipping element..." << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D>
    auto readTetrahedra(
        std::ifstream & fileStream, mesh_t<topology::tetrahedron_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, int N_cells,
        const std::vector<vertex_t> & vertices) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            if (cell_type == 4) {
                readTetrahedron(fileStream, mesh, geometry, vertices);
            } else {
                std::cout << "Not a tetrahedron, skipping element..." << std::endl;
            }
        }

        // all done
        return;
    }

    template <int N, int D>
    auto readElements(
        std::ifstream & fileStream, mesh_t<topology::simplex_t<N>, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, int N_cells,
        const std::vector<vertex_t> & vertices) -> void;

    template <int D>
    auto readElements(
        std::ifstream & fileStream, mesh_t<topology::segment_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, int N_cells,
        const std::vector<vertex_t> & vertices) -> void
    {
        return readSegments(fileStream, mesh, geometry, N_cells, vertices);
    }

    template <int D>
    auto readElements(
        std::ifstream & fileStream, mesh_t<topology::tetrahedron_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, int N_cells,
        const std::vector<vertex_t> & vertices) -> void
    {
        return readTetrahedra(fileStream, mesh, geometry, N_cells, vertices);
    }

    template <int D>
    auto readElements(
        std::ifstream & fileStream, mesh_t<topology::triangle_t, D> & mesh,
        mito::geometry::geometry_t<D> & geometry, int N_cells,
        const std::vector<vertex_t> & vertices) -> void
    {
        return readTriangles(fileStream, mesh, geometry, N_cells, vertices);
    }

    template <class cellT, int D>
    auto summit(std::ifstream & fileStream, mito::geometry::geometry_t<D> & geometry) -> auto
    {
        std::cout << "Loading summit mesh..." << std::endl;
        assert(fileStream.is_open());

        // read dimension of physical space
        int dim = 0;
        fileStream >> dim;

        // assert this mesh object is of same dimension of the mesh being read
        assert(int(D) == dim);

        // instantiate mesh
        auto mesh = mito::mesh::mesh<cellT, D>(geometry);

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
        readVertices<D>(fileStream, geometry, N_vertices, vertices);

        // read the cells
        readElements(fileStream, mesh, geometry, N_cells, vertices);

        // sanity check: the number of vertices in the map is N_vertices
        vertices.shrink_to_fit();
        assert(vertices.size() == static_cast<size_t>(N_vertices));

        // sanity check: the number of cells of highest dimension in the map is N_cells
        assert(mesh.template nCells() == N_cells);

        // sanity check: run sanity check for all mesh simplices in cascade
        assert(mesh.sanityCheck());

        // all done
        return mesh;
    }

}    // namespace mito::mesh

#endif    // mito_mesh_summit_h