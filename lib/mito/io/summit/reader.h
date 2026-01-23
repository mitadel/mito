// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::io::summit {
    template <int D, geometry::coordinates_c coordT>
    requires(coordT::dim == D)
    auto readVertices(
        std::ifstream & fileStream, geometry::coordinate_system_t<coordT> & coordinate_system,
        int N_vertices, std::vector<geometry::node_t<D>> & nodes) -> void
    {
        // fill in nodes
        for (int n = 0; n < N_vertices; ++n) {
            // instantiate new point
            tensor::vector_t<D> coordinates;
            for (int d = 0; d < D; ++d) {
                // read point coordinates
                fileStream >> coordinates[d];
            }

            // the type of coordinates
            using coordinates_type = coordT;

            // instantiate a new node
            auto node = mito::geometry::node(coordinate_system, coordinates_type(coordinates));

            // add node to {nodes}
            nodes.push_back(node);
        }

        // all done
        return;
    }

    template <GalerkinMeshType galerkinT, geometry::geometric_simplex_c cellT>
    auto readElement(
        std::ifstream & fileStream, mesh::mesh_t<cellT> & mesh,
        const std::vector<geometry::node_t<cellT::dim>> & nodes) -> void
    {
        // get the number of vertices
        constexpr int N = cellT::n_vertices;

        // the element connectivity
        std::array<int, N> index;
        // for each node
        for (int i = 0; i < N; ++i) {
            // read from file the id of the node
            int id = 0;
            fileStream >> id;
            // start from zero (the summit format starts counting from one)
            --id;
            // take a note of it
            index[i] = id;
        }

        // if it is a continuous Galerkin mesh
        if constexpr (galerkinT == CG) {

            // helper function to call the mesh insert method with {N} nodes
            constexpr auto _insert = []<size_t... I>(
                                         mesh::mesh_t<cellT> & mesh, const std::array<int, N> index,
                                         const std::vector<geometry::node_t<cellT::dim>> & nodes,
                                         std::index_sequence<I...>) {
                // insert in the mesh a geometric simplex with these nodes
                mesh.insert({ nodes[index[I]]... });
            };

            // insert the nodes in the mesh
            _insert(mesh, index, nodes, std::make_index_sequence<N>{});

        }
        // otherwise
        else {
            // assert that it is then a discontinuous Galerkin mesh
            static_assert(galerkinT == DG);

            // helper function to call the mesh insert method with {N} nodes
            constexpr auto _insert = []<size_t... I>(
                                         mesh::mesh_t<cellT> & mesh, const std::array<int, N> index,
                                         const std::vector<geometry::node_t<cellT::dim>> & nodes,
                                         std::index_sequence<I...>) {
                // insert in the mesh a geometric simplex with a new instance of the nodes riding on
                // same vertex and same point
                mesh.insert({ geometry::node_t<cellT::dim>(
                    nodes[index[I]]->vertex(), nodes[index[I]]->point())... });
            };

            // insert the nodes in the mesh
            _insert(mesh, index, nodes, std::make_index_sequence<N>{});
        }

        // QUESTION: Can the label be more than one?
        // read label for cell
        // TOFIX: Ignored for now
        std::string cell_set_id;
        fileStream >> cell_set_id;

        // all done
        return;
    }

    template <GalerkinMeshType galerkinT, class cellT>
    auto readElements(
        std::ifstream & fileStream, mesh::mesh_t<cellT> & mesh, int N_cells,
        const std::vector<geometry::node_t<cellT::dim>> & nodes) -> void
    {
        // for each element
        for (int i = 0; i < N_cells; ++i) {
            // read the cell type from file
            int cell_type = 0;
            fileStream >> cell_type;

            // if the cell type read from file matches with the cell of the mesh to be populated
            if (cell_type == summit::cell<cellT>::type) {
                // read the element and insert it in the mesh
                readElement<galerkinT>(fileStream, mesh, nodes);
            }
        }

        // all done
        return;
    }

    template <class cellT, GalerkinMeshType galerkinT = CG, geometry::coordinates_c coordT>
    auto reader(
        std::ifstream & fileStream, geometry::coordinate_system_t<coordT> & coordinate_system)
        -> mesh::mesh_t<cellT>
    requires(utilities::same_dim_c<cellT, coordT>)
    {
        if (!fileStream.is_open()) {
            throw std::runtime_error("reader: Mesh file could not be opened");
        }

        // make a channel
        journal::info_t channel("mito.summit.mesh_reader");

        // report
        channel << "Loading summit mesh..." << journal::endl;

        // read dimension of physical space
        int dim = 0;
        fileStream >> dim;

        // the dimension of the physical space
        constexpr int D = cellT::dim;

        // assert this mesh object is of same dimension of the mesh being read
        assert(D == dim);

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
        readElements<galerkinT>(fileStream, mesh, N_cells, nodes);

        // sanity check: the number of nodes in the map is N_vertices
        nodes.shrink_to_fit();
        assert(std::size(nodes) == static_cast<size_t>(N_vertices));

        // sanity check: the number of cells of highest dimension in the map is N_cells
        assert(mesh.nCells() == N_cells);

        // all done
        return mesh;
    }

}    // namespace io::summit


// end of file
