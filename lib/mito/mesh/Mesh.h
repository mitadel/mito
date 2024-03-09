// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    template <class cellT /* the type of cell */>
    class Mesh {

      public:
        // typedef for cell type
        using cell_type = cellT;
        // TOFIX: below we make the assumption that the cell is always simplicial, remove this
        //  assumption.
        // typedef for simplex type
        using simplex_type = cell_type::simplex_type;
        // publish the order of the cell
        static constexpr int order = topology::order<simplex_type>();
        // publish the number of vertices per element
        static constexpr int n_vertices = topology::n_vertices<simplex_type>();
        // the type of vertex
        using vertex_type = cell_type::vertex_type;
        // a collection of nodes
        using node_type = cell_type::node_type;
        // a collection of nodes
        using nodes_type = cell_type::nodes_type;
        // publish the dimension of physical space
        static constexpr int dim = cell_type::dim;
        // typedef for a collection of cells
        // QUESTION: we may consider switching this to a container that is faster to iterate on
        //  e.g. {vector}, if we decide to base {Manifold} on a {Mesh}. In this case, in fact,
        //  iterating on a manifold would iterate on the mesh elements. It is true that
        //  inserting/erasing elements in a vector is more expensive than in a set. However, we
        //  insert/erase much less frequently than we iterate. Also, this vector does not contain
        //  the full cell objects, but just their addresses, so perhaps it would not even be that
        //  expensive to allocate/deallocate the memory.
        using cells_type = utilities::segmented_vector_t<cell_type>;
        // iterator to the cells datastructure
        using cells_iterator = cells_type::iterator;

      private:
        // get the order of the cell
        static constexpr int N = order;
        // the dimension of physical space
        static constexpr int D = dim;
        // get the family this cell type belongs to (e.g. geometric simplices)
        template <int I, int DD>
        using cell_family_type = typename cell_type::cell_family_type<I, DD>;
        // get the topological family this cell type belongs to (e.g. simplicial cells)
        template <int I>
        using cell_topological_family_type = typename topology::cell_family<simplex_type, I>;
        // id type of unoriented cell
        using cell_id_type = utilities::index_t<cell_type>;
        // this map maps a simplex id to a tuple of two integers counting how many times a simplex
        // appears with - or + orientation
        using orientation_map_type = std::unordered_map<cell_id_type, std::array<int, 2>>;

      public:
        // default constructor
        inline Mesh()
        requires(N <= D)
            : _cells(100)
        {}

        inline ~Mesh() = default;

        // move constructor
        inline Mesh(Mesh &&) noexcept = default;

      private:
        // delete copy constructor
        Mesh(const Mesh &) = delete;

        // delete assignment operator
        Mesh & operator=(const Mesh &) = delete;

        // delete move assignment operator
        Mesh & operator=(Mesh &&) noexcept = delete;

      private:
        inline auto _register_cell_orientation(const cell_type & cell) -> void
        {
            // loop on the subcells of {cell}
            for (const auto & subcell : cell.simplex()->composition()) {
                // increment the orientations count for this cell footprint id, depending on the
                // orientation
                (subcell->orientation() == +1 ? _orientations[subcell->footprint().id()][0] += 1 :
                                                _orientations[subcell->footprint().id()][1] += 1);
            }

            // all done
            return;
        }

      public:
        inline auto nCells() const noexcept -> int
        {
            // all done
            return std::size(_cells);
        }

        inline auto cells() const noexcept -> const cells_type &
        {
            // all done
            return _cells;
        }

        inline auto cells() noexcept -> cells_type &
        {
            // all done
            return _cells;
        }

        // erase cell at location {cell}
        inline auto erase(cells_iterator & cell) -> void
        {
            // erase the cell from the mesh
            bool cell_was_erased = _cells.erase(cell);
            // if the cell was in fact erased from the cell
            if (cell_was_erased) {
                // loop on the subcells of {cell}
                for (const auto & subcell : cell->simplex()->composition()) {
                    // decrement the orientations count for this cell footprint id, depending on
                    // the orientation
                    (subcell->orientation() == +1 ?
                         _orientations[subcell->footprint().id()][0] -= 1 :
                         _orientations[subcell->footprint().id()][1] -= 1);

                    // cleanup orientation map
                    if (_orientations[subcell->footprint().id()] == std::array<int, 2>{ 0, 0 }) {
                        _orientations.erase(subcell->footprint().id());
                    }
                }
            }

            // all done
            return;
        }

        inline auto isOnBoundary(const cell_topological_family_type<N - 1> & cell) const -> bool
        {
            // count how many times this oriented cell occurs in the mesh with opposite orientation
            int count = 0;
            (cell->orientation() == -1 ? count = _orientations.at(cell->footprint().id())[0] :
                                         count = _orientations.at(cell->footprint().id())[1]);

            // the cell is on the boundary if it never occurs in the mesh with opposite
            // orientation
            if (count == 0) {
                return true;
            }

            return false;
        }

        /**
         * @brief Returns a mesh with all boundary cells of dimension I
         */
        template <int I = N - 1>
        inline auto boundary_size() const -> int
        requires(N > 0 && I >= 0)
        {
            // number of boundary cells
            int count = 0;

            // loop on the (N-1)-dimensional cells
            for (const auto & cell : cells()) {
                // loop on the subcells of {cell}
                for (const auto & subcell : cell.composition()) {
                    // if {subcell} does not have a counterpart in {topology} with opposite
                    // orientation
                    if (isOnBoundary(subcell)) {
                        // increment counter for boundary cells
                        ++count;
                    }
                }
            }

            // return the count of boundary cells
            return count;
        }

        /**
         * @brief Returns a mesh with all boundary cells of dimension I
         */
        template <int I = N - 1>
        inline auto boundary() const -> Mesh<cell_family_type<I, D>> const
        requires(N > 0 && I >= 0)
        {
            // instantiate a new mesh for the boundary elements
            Mesh<cell_family_type<I, D>> boundary_mesh;

            // loop on the mesh cells
            for (const auto & cell : cells()) {
                // loop on the topological composition of {cell}
                for (const auto & subcell : cell.simplex()->composition()) {
                    // if {subcell} does not have a counterpart in the mesh with opposite
                    // orientation
                    if (isOnBoundary(subcell)) {
                        // add {cell} to the boundary mesh
                        boundary_mesh.insert(
                            mito::geometry::geometric_simplex<D>(subcell, cell.nodes()));
                    }
                }
            }

            // return the boundary mesh
            return boundary_mesh;
        }

        // insert {cell} in mesh
        inline auto insert(const cell_type & cell) -> cells_iterator
        requires(N > 0)
        {
            // register {cell} in the orientation map
            _register_cell_orientation(cell);

            // add the cell to the collection of cells
            return _cells.emplace_back(cell);
        }

        // build a cell based with nodes {nodes} and insert it in mesh
        inline auto insert(const nodes_type & nodes) -> cells_iterator
        requires(N > 0)
        {

            // instantiate cell and add it to the collection of cells
            auto cell = _cells.emplace_back(nodes);

            // register {cell} in the orientation map
            _register_cell_orientation(*cell);

            // all done
            return cell;
        }

        // insert {cell} in mesh
        inline auto insert(const cell_type & cell) -> cells_iterator
        requires(N == 0)
        {
            // add the cell to the collection of cells
            return _cells.emplace_back(cell);
        }

      private:
        // container to store the mesh cells
        cells_type _cells;

        // container to store how many times a simplex appears with a given orientation
        orientation_map_type _orientations;
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
