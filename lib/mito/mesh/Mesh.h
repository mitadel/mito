// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    template <class cellT /* the type of cell */, int D /* spatial dimension */>
    class Mesh {

      private:
        // typedef for cell type
        using cell_t = cellT;
        // get the order of the cell
        static constexpr int N = cellT::resource_t::order;
        // get the family this cell type belongs to (e.g. simplicial cells)
        template <int I>
        using cell_family_t = typename cellT::resource_t::cell_family_t<I>;
        // typedef for geometry type
        using geometry_t = mito::geometry::geometry_t<D>;
        // typedef for a collection of cells
        using cells_t = element_set_t<cell_t>;
        // this map maps a simplex id to a tuple of two integers counting how many times a simplex
        // appears with - or + orientation
        using orientation_map_t =
            std::unordered_map<topology::unoriented_simplex_id_t, std::array<int, 2>>;

      public:
        // default constructor
        inline Mesh(geometry_t & geometry)
        requires(N <= D)
            : _geometry(geometry), _cells() {};

        inline ~Mesh() {}

        // move constructor
        inline Mesh(Mesh &&) = default;

      private:
        // delete copy constructor
        Mesh(const Mesh &) = delete;

        // delete assignment operator
        const Mesh & operator=(const Mesh &) = delete;

        // delete move assignment operator
        const Mesh & operator=(const Mesh &&) = delete;

      public:
        inline auto sanityCheck() -> bool
        {
#if 0
            // print summary
            std::cout << "Mesh composition: " << std::endl;
            std::cout << _cells.size() << " cells embedded in " << D << " dimension " << std::endl;
#endif

            // sanity check: each cell is self-consistent
            for (const auto & cell : _cells) {
                if (!cell->sanityCheck()) {
                    return false;
                }
            }

            return true;
        }

        inline auto nCells() const -> int
        {
            // all done
            return _cells.size();
        }

        inline auto cells() const -> const auto &
        {
            // all done
            return _cells;
        }

        // TOFIX: let {Mesh} answer the question for now, although this is not a
        // question for {Mesh}, but a question for {Geometry}
        inline auto nodes() const -> const auto &
        {
            // return the collection of nodes
            return _geometry.nodes();
        }

        // TOFIX: let {Mesh} answer the question for now, although this is not a
        // question for {Mesh}, but a question for {Geometry}
        auto point(const vertex_t & vertex) -> const point_t<D> &
        {
            // return the point corresponding to vertex {vertex}
            return _geometry.point(vertex);
        }

        inline auto erase(const cell_t & cell) -> void
        {

            // loop on the subcells of {cell}
            for (const auto & subcell : cell->composition()) {
                // decrement the orientations count for this cell footprint id, depending on the
                // orientation
                (subcell->orientation() ? _orientations[subcell->footprint()->id()][0] -= 1 :
                                          _orientations[subcell->footprint()->id()][1] -= 1);

                // cleanup orientation map
                if (_orientations[subcell->footprint()->id()] == std::array<int, 2> { 0, 0 }) {
                    _orientations.erase(subcell->footprint()->id());
                }
            }

            // erase the cell from the mesh
            _cells.erase(cell);

            // TOFIX: is this the only reason that {_geometry} cannot be a const reference?
            // erase cell from topology
            _geometry.topology().erase(cell);

            // all done
            return;
        }

        inline auto isOnBoundary(const cell_family_t<N - 1> & cell) -> bool
        {
            // count how many times this oriented cell occurs in the mesh with opposite orientation
            int count = 0;
            (!cell->orientation() ? count = _orientations[cell->footprint()->id()][0] :
                                    count = _orientations[cell->footprint()->id()][1]);

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
        inline auto boundary() -> Mesh<cell_family_t<I>, D>
        requires(I == N - 1)
        {
            // instantiate a new mesh for the boundary elements
            Mesh<cell_family_t<I>, D> boundary_mesh(_geometry);

            // loop on the (I+1) dimensional cells
            for (const auto & cell : cells()) {
                // loop on the subcells of {cell}
                for (const auto & subcell : cell->composition()) {
                    // if {subcell} does not have a counterpart in {topology} with opposite
                    // orientation
                    if (isOnBoundary(subcell)) {
                        // add {subcell} to the boundary mesh
                        boundary_mesh.insert(subcell);
                    }
                }
            }

            // return the boundary mesh
            return boundary_mesh;
        }

      public:
        inline auto insert(const cell_t & cell) -> void
        {
            // add the cell to the set of cells with same dimension
            _cells.insert(cell);

            // loop on the subcells of {cell}
            for (const auto & subcell : cell->composition()) {
                // increment the orientations count for this cell footprint id, depending on the
                // orientation
                (subcell->orientation() ? _orientations[subcell->footprint()->id()][0] += 1 :
                                          _orientations[subcell->footprint()->id()][1] += 1);
            }

            // all done
            return;
        }

      public:
        // accessor to geometry
        auto geometry() -> geometry_t & { return _geometry; }

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
