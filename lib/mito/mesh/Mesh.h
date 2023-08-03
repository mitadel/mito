// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    template <class cellT /* the type of cell */, int D /* spatial dimension */>
    class Mesh {

      public:
        // publish the order of the cell
        static constexpr int order = cellT::resource_t::order;
        // publish the number of vertices per element
        static constexpr int n_vertices = cellT::resource_t::n_vertices;

      private:
        // typedef for cell type
        using cell_t = cellT;
        // get the order of the cell
        static constexpr int N = cellT::resource_t::order;
        // get the family this cell type belongs to (e.g. simplicial cells)
        template <int I>
        using cell_family_t = typename cellT::resource_t::template cell_family_t<I>;
        // typedef for geometry type
        using geometry_t = geometry::geometry_t<D>;
        // typedef for a collection of cells
        using cells_t = element_set_t<cell_t>;
        // this map maps a simplex id to a tuple of two integers counting how many times a simplex
        // appears with - or + orientation
        using orientation_map_t =
            std::unordered_map<topology::unoriented_simplex_id_t<D>, std::array<int, 2>>;

      public:
        // default constructor
        inline Mesh(geometry_t & geometry)
        requires(N <= D)
            : _geometry(geometry), _cells() {};

        inline ~Mesh() {}

        // move constructor
        inline Mesh(Mesh &&) noexcept = default;

      private:
        // delete copy constructor
        Mesh(const Mesh &) = delete;

        // delete assignment operator
        Mesh & operator=(const Mesh &) = delete;

        // delete move assignment operator
        Mesh & operator=(Mesh &&) = delete;

      private:
        template <int I, int J>
        inline auto _insert_subcells(
            Mesh<cell_family_t<I>, D> & boundary_mesh, const cell_family_t<J> & cell) const -> void
        requires(I == J)
        {
            // add {subcell} to the boundary mesh
            boundary_mesh.insert(cell);

            // all done
            return;
        }

        template <int I, int J>
        inline auto _insert_subcells(
            Mesh<cell_family_t<I>, D> & boundary_mesh, const cell_family_t<J> & cell) const -> void
        requires(I < J)
        {
            // loop on the subcells of {cell}
            for (const auto & subcell : cell->composition()) {
                // recursively add subcells of {subcell} to {boundary_mesh}
                _insert_subcells(boundary_mesh, subcell);
            }

            // all done
            return;
        }

      public:
        inline auto sanityCheck() const -> bool
        {
#if 0
            // print summary
            std::cout << "Mesh composition: " << std::endl;
            std::cout << std::size(_cells) << " cells embedded in " << D << " dimension " << std::endl;
#endif

            // sanity check: each cell is self-consistent
            for (const auto & cell : _cells) {
                if (!cell->sanityCheck()) {
                    return false;
                }
            }

            return true;
        }

        inline auto nCells() const noexcept -> int
        {
            // all done
            return std::size(_cells);
        }

        inline auto cells() const noexcept -> const cells_t &
        {
            // all done
            return _cells;
        }

        inline auto erase(const cell_t & cell) -> void
        {
            // loop on the subcells of {cell}
            for (const auto & subcell : cell->composition()) {
                // decrement the orientations count for this cell footprint id, depending on the
                // orientation
                (subcell->orientation() ? _orientations[subcell->footprint().id()][0] -= 1 :
                                          _orientations[subcell->footprint().id()][1] -= 1);

                // cleanup orientation map
                if (_orientations[subcell->footprint().id()] == std::array<int, 2> { 0, 0 }) {
                    _orientations.erase(subcell->footprint().id());
                }
            }

            // erase the cell from the mesh
            _cells.erase(cell);

            // ask the topology to erase the cell
            _geometry.topology().template erase<D>(cell.id());

            // all done
            return;
        }

        inline auto isOnBoundary(const cell_family_t<N - 1> & cell) const -> bool
        {
            // count how many times this oriented cell occurs in the mesh with opposite orientation
            int count = 0;
            (!cell->orientation() ? count = _orientations.at(cell->footprint().id())[0] :
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
        requires(I >= 0)
        {
            // number of boundary cells
            int count = 0;

            // loop on the (N-1)-dimensional cells
            for (const auto & cell : cells()) {
                // loop on the subcells of {cell}
                for (const auto & subcell : cell->composition()) {
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
        // TOFIX: should this be const?
        template <int I = N - 1>
        inline auto boundary() const -> Mesh<cell_family_t<I>, D>
        requires(I >= 0)
        {
            // instantiate a new mesh for the boundary elements
            Mesh<cell_family_t<I>, D> boundary_mesh(_geometry);

            // loop on the (N-1)-dimensional cells
            for (const auto & cell : cells()) {
                // loop on the subcells of {cell}
                for (const auto & subcell : cell->composition()) {
                    // if {subcell} does not have a counterpart in {topology} with opposite
                    // orientation
                    if (isOnBoundary(subcell)) {
                        _insert_subcells(boundary_mesh, subcell);
                    }
                }
            }

            // return the boundary mesh
            return boundary_mesh;
        }

        inline auto insert(const cell_t & cell) -> void
        requires(N >= 1)
        {
            // add the cell to the set of cells with same dimension
            _cells.insert(cell);

            // loop on the subcells of {cell}
            for (const auto & subcell : cell->composition()) {
                // increment the orientations count for this cell footprint id, depending on the
                // orientation
                (subcell->orientation() ? _orientations[subcell->footprint().id()][0] += 1 :
                                          _orientations[subcell->footprint().id()][1] += 1);
            }

            // all done
            return;
        }

        // QUESTION: specialization of method {insert} for a mesh of vertices
        //           not sure if supporting meshes of 0-simplices makes sense...
        inline auto insert(const cell_t & cell) -> void
        requires(N == 0)
        {
            // add the cell to the set of cells with same dimension
            _cells.insert(cell);

            // all done
            return;
        }

      public:
        // accessor to geometry
        auto geometry() noexcept -> geometry_t & { return _geometry; }

        // const accessor to geometry
        auto geometry() const noexcept -> const geometry_t & { return _geometry; }

      private:
        // a reference to the geometry where the cells are embedded
        geometry_t & _geometry;

        // container to store the mesh cells
        cells_t _cells;

        // container to store how many times a simplex appears with a given orientation
        orientation_map_t _orientations;
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
