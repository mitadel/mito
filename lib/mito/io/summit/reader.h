// code guard
#if !defined(mito_io_summit_reader_h)
#define mito_io_summit_reader_h


namespace mito::io::summit {
    template <int D, geometry::CoordinateType coordT>
    auto readVertices(
        std::ifstream & fileStream, geometry::geometry_t<D, coordT> & geometry, int N_vertices,
        std::vector<topology::vertex_t> & vertices) -> void
    {
        // fill in vertices
        for (int n = 0; n < N_vertices; ++n) {
            // instantiate new point
            vector_t<D> coordinates;
            for (int d = 0; d < D; ++d) {
                // read point coordinates
                fileStream >> coordinates[d];
            }

            // instantiate a new node
            auto vertex = mito::geometry::node(
                geometry, mito::geometry::coordinates_t<D, coordT>(coordinates));

            // add vertex to {vertices}
            vertices.push_back(vertex);
        }

        // all done
        return;
    }

    template <int D, geometry::CoordinateType coordT>
    auto readSegment(
        std::ifstream & fileStream, mesh::mesh_t<topology::segment_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry,
        const std::vector<topology::vertex_t> & vertices) -> void
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

    template <int D, geometry::CoordinateType coordT>
    auto readTriangle(
        std::ifstream & fileStream, mesh::mesh_t<topology::triangle_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry,
        const std::vector<topology::vertex_t> & vertices) -> void
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

        const auto & cell = topology.triangle({ vertex0, vertex1, vertex2 });
        mesh.insert(cell);

        // QUESTION: Can the label be more than one?
        // read label for cell
        // TOFIX: Ignored for now
        std::string cell_label;
        fileStream >> cell_label;

        // all done
        return;
    }

    template <int D, geometry::CoordinateType coordT>
    inline auto orientation(
        const geometry::geometry_t<D, coordT> & geometry, const topology::vertex_t & v0,
        const topology::vertex_t & v1, const topology::vertex_t & v2, const topology::vertex_t & v3)
        -> bool
    {
        auto p0 = geometry.point(v0)->coordinates();
        auto p1 = geometry.point(v1)->coordinates();
        auto p2 = geometry.point(v2)->coordinates();
        auto p3 = geometry.point(v3)->coordinates();

        auto dir1 = p1 - p0;
        auto dir2 = p2 - p0;
        auto dir3 = p3 - p0;

        return pyre::tensor::cross(dir1, dir2) * dir3 > 0;
    }

    template <int D, geometry::CoordinateType coordT>
    auto readTetrahedron(
        std::ifstream & fileStream, mesh::mesh_t<topology::tetrahedron_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry,
        const std::vector<topology::vertex_t> & vertices) -> void
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

        assert(orientation(geometry, vertex0, vertex1, vertex2, vertex3));

        const auto & cell = topology.tetrahedron({ vertex0, vertex1, vertex2, vertex3 });
        mesh.insert(cell);

        // QUESTION: Can the label be more than one?
        // read label for cell
        // TOFIX: Ignored for now
        std::string cell_set_id;
        fileStream >> cell_set_id;

        // all done
        return;
    }

    template <int D, geometry::CoordinateType coordT>
    auto readSegments(
        std::ifstream & fileStream, mesh::mesh_t<topology::segment_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry, int N_cells,
        const std::vector<topology::vertex_t> & vertices) -> void
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

    template <int D, geometry::CoordinateType coordT>
    auto readTriangles(
        std::ifstream & fileStream, mesh::mesh_t<topology::triangle_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry, int N_cells,
        const std::vector<topology::vertex_t> & vertices) -> void
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

    template <int D, geometry::CoordinateType coordT>
    auto readTetrahedra(
        std::ifstream & fileStream, mesh::mesh_t<topology::tetrahedron_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry, int N_cells,
        const std::vector<topology::vertex_t> & vertices) -> void
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

    template <int D, geometry::CoordinateType coordT>
    auto readElements(
        std::ifstream & fileStream, mesh::mesh_t<topology::segment_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry, int N_cells,
        const std::vector<topology::vertex_t> & vertices) -> void
    {
        return readSegments(fileStream, mesh, geometry, N_cells, vertices);
    }

    template <int D, geometry::CoordinateType coordT>
    auto readElements(
        std::ifstream & fileStream, mesh::mesh_t<topology::tetrahedron_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry, int N_cells,
        const std::vector<topology::vertex_t> & vertices) -> void
    {
        return readTetrahedra(fileStream, mesh, geometry, N_cells, vertices);
    }

    template <int D, geometry::CoordinateType coordT>
    auto readElements(
        std::ifstream & fileStream, mesh::mesh_t<topology::triangle_t, D, coordT> & mesh,
        geometry::geometry_t<D, coordT> & geometry, int N_cells,
        const std::vector<topology::vertex_t> & vertices) -> void
    {
        return readTriangles(fileStream, mesh, geometry, N_cells, vertices);
    }

    template <class cellT, int D, geometry::CoordinateType coordT>
    auto reader(std::ifstream & fileStream, geometry::geometry_t<D, coordT> & geometry)
        -> mesh::mesh_t<cellT, D, coordT>
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
        auto mesh = mesh::mesh<cellT, D, coordT>(geometry);

        // read number of vertices
        int N_vertices = 0;
        fileStream >> N_vertices;
        // reserve space for vertices
        std::vector<topology::vertex_t> vertices;
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
        readVertices(fileStream, geometry, N_vertices, vertices);

        // read the cells
        readElements(fileStream, mesh, geometry, N_cells, vertices);

        // sanity check: the number of vertices in the map is N_vertices
        vertices.shrink_to_fit();
        assert(std::size(vertices) == static_cast<size_t>(N_vertices));

        // sanity check: the number of cells of highest dimension in the map is N_cells
        assert(mesh.nCells() == N_cells);

        // sanity check: run sanity check for all mesh simplices in cascade
        assert(mesh.sanityCheck());

        // all done
        return mesh;
    }

}    // namespace io::summit

#endif    // mito_io_summit_reader_h