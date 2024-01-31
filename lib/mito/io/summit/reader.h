// code guard
#if !defined(mito_io_summit_reader_h)
#define mito_io_summit_reader_h


namespace mito::io::summit {
    template <int D, geometry::CoordinateType coordT>
    auto readVertices(
        std::ifstream & fileStream, geometry::coordinate_system_t<D, coordT> & coordinate_system,
        int N_vertices, std::vector<geometry::node_t<D>> & nodes) -> void
    {
        // fill in nodes
        for (int n = 0; n < N_vertices; ++n) {
            // instantiate new point
            vector_t<D> coordinates;
            for (int d = 0; d < D; ++d) {
                // read point coordinates
                fileStream >> coordinates[d];
            }

            // instantiate a new node
            auto node = mito::geometry::node(
                coordinate_system, mito::geometry::coordinates_t<D, coordT>(coordinates));

            // add node to {nodes}
            nodes.push_back(node);
        }

        // all done
        return;
    }

    template <int D>
    auto readSegment(
        std::ifstream & fileStream, mesh::mesh_t<geometry::segment_t<D>> & mesh,
        const std::vector<geometry::node_t<D>> & nodes) -> void
    {
        int index0 = 0;
        fileStream >> index0;
        --index0;

        int index1 = 0;
        fileStream >> index1;
        --index1;

        const auto & node_0 = nodes[index0];
        const auto & node_1 = nodes[index1];

        mesh.insert({ node_0, node_1 });

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
        std::ifstream & fileStream, mesh::mesh_t<geometry::triangle_t<D>> & mesh,
        const std::vector<geometry::node_t<D>> & nodes) -> void
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

        const auto & node_0 = nodes[index0];
        const auto & node_1 = nodes[index1];
        const auto & node_2 = nodes[index2];

        mesh.insert({ node_0, node_1, node_2 });

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
        std::ifstream & fileStream, mesh::mesh_t<geometry::tetrahedron_t<D>> & mesh,
        const std::vector<geometry::node_t<D>> & nodes) -> void
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

        const auto & node_0 = nodes[index0];
        const auto & node_1 = nodes[index1];
        const auto & node_2 = nodes[index2];
        const auto & node_3 = nodes[index3];

        mesh.insert({ node_0, node_1, node_2, node_3 });

        // QUESTION: Can the label be more than one?
        // read label for cell
        // TOFIX: Ignored for now
        std::string cell_set_id;
        fileStream >> cell_set_id;

        // all done
        return;
    }

    template <int D>
    auto readSegments(
        std::ifstream & fileStream, mesh::mesh_t<geometry::segment_t<D>> & mesh, int N_cells,
        const std::vector<geometry::node_t<D>> & nodes) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            // TODO: add read segment
            if (cell_type == 2) {
                readSegment(fileStream, mesh, nodes);
            } else {
                std::cout << "Not a segment, skipping element..." << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D>
    auto readTriangles(
        std::ifstream & fileStream, mesh::mesh_t<geometry::triangle_t<D>> & mesh, int N_cells,
        const std::vector<geometry::node_t<D>> & nodes) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            // TODO: add read segment
            if (cell_type == 3) {
                readTriangle(fileStream, mesh, nodes);
            } else {
                std::cout << "Not a triangle, skipping element..." << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D>
    auto readTetrahedra(
        std::ifstream & fileStream, mesh::mesh_t<geometry::tetrahedron_t<D>> & mesh, int N_cells,
        const std::vector<geometry::node_t<D>> & nodes) -> void
    {
        for (int i = 0; i < N_cells; ++i) {
            int cell_type = 0;
            fileStream >> cell_type;

            if (cell_type == 4) {
                readTetrahedron(fileStream, mesh, nodes);
            } else {
                std::cout << "Not a tetrahedron, skipping element..." << std::endl;
            }
        }

        // all done
        return;
    }

    template <int D>
    auto readElements(
        std::ifstream & fileStream, mesh::mesh_t<geometry::segment_t<D>> & mesh, int N_cells,
        const std::vector<geometry::node_t<D>> & nodes) -> void
    {
        return readSegments(fileStream, mesh, N_cells, nodes);
    }

    template <int D>
    auto readElements(
        std::ifstream & fileStream, mesh::mesh_t<geometry::tetrahedron_t<D>> & mesh, int N_cells,
        const std::vector<geometry::node_t<D>> & nodes) -> void
    {
        return readTetrahedra(fileStream, mesh, N_cells, nodes);
    }

    template <int D>
    auto readElements(
        std::ifstream & fileStream, mesh::mesh_t<geometry::triangle_t<D>> & mesh, int N_cells,
        const std::vector<geometry::node_t<D>> & nodes) -> void
    {
        return readTriangles(fileStream, mesh, N_cells, nodes);
    }

    template <class cellT, int D, geometry::CoordinateType coordT>
    auto reader(
        std::ifstream & fileStream, geometry::coordinate_system_t<D, coordT> & coordinate_system)
        -> mesh::mesh_t<cellT>
    requires(mesh::mesh_t<cellT>::dim == D)
    {
        if (!fileStream.is_open()) {
            throw std::runtime_error("reader: Mesh file could not be opened");
        }

        std::cout << "Loading summit mesh..." << std::endl;

        // read dimension of physical space
        int dim = 0;
        fileStream >> dim;

        // assert this mesh object is of same dimension of the mesh being read
        assert(int(D) == dim);

        // instantiate mesh
        auto mesh = mesh::mesh<cellT>();

        // read number of vertices
        int N_vertices = 0;
        fileStream >> N_vertices;
        // reserve space for nodes
        std::vector<geometry::node_t<D>> nodes;
        nodes.reserve(N_vertices);

        // read number of cells
        int N_cells = 0;
        fileStream >> N_cells;

        // read number of cell types
        int N_cell_types = 0;
        fileStream >> N_cell_types;

        // QUESTION: Not sure that we need this...
        assert(N_cell_types == 1);

        // read the nodes
        readVertices(fileStream, coordinate_system, N_vertices, nodes);

        // read the cells
        readElements(fileStream, mesh, N_cells, nodes);

        // sanity check: the number of nodes in the map is N_vertices
        nodes.shrink_to_fit();
        assert(std::size(nodes) == static_cast<size_t>(N_vertices));

        // sanity check: the number of cells of highest dimension in the map is N_cells
        assert(mesh.nCells() == N_cells);

        // all done
        return mesh;
    }

}    // namespace io::summit

#endif    // mito_io_summit_reader_h