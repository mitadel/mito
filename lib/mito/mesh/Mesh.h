// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    template <
        int D /*spatial dimension*/, template <int> class cellT,
        int N /*highest order of simplices */>
    requires(N <= D)
    class Mesh {

      private:
        template <int I>
        using cell_t = cellT<I>;

        // typedef for a collection of cells of dimension I
        template <size_t I>
        using cell_collection = element_set_t<cell_t<I>>;

        // cell_collection<I>... expands to:
        // cell_set_t<cell_t<1>>, ..., cell_set_t<cell_t<N>>
        template <typename = std::make_index_sequence<N + 1>>
        struct cell_tuple;

        template <size_t... I>
        struct cell_tuple<std::index_sequence<I...>> {
            using type = std::tuple<cell_collection<I>...>;
        };

        // this expands to:
        // tuple<cell_set_t<cell_t<0>>,
        //      cell_set_t<cell_t<1>>, ...,
        //      cell_set_t<cell_t<N>>
        using cell_tuple_t = typename cell_tuple<>::type;

        using geometry_t = mito::geometry::geometry_t<D>;

      public:
        // default constructor
        inline Mesh(geometry_t & geometry) : _geometry(geometry), _cells() {};

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
            std::cout << "0: " << std::get<0>(_cells).size() << " cells " << std::endl;
            std::cout << "1: " << std::get<1>(_cells).size() << " cells " << std::endl;
            std::cout << "2: " << std::get<2>(_cells).size() << " cells " << std::endl;
#endif

            // sanity check: each cell is self-consistent
            for (const auto & cell : std::get<N>(_cells)) {
                if (!cell->sanityCheck()) {
                    return false;
                }
            }

            return true;
        }

        template <int I = N>
        inline auto nCells() const -> int
        requires(I <= N)
        {
            // all done
            return std::get<I>(_cells).size();
        }

        template <int I = N>
        inline auto cells() const -> const auto & requires(I <= N) {
                                                      // all done
                                                      return std::get<I>(_cells);
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

        template <int I>
        inline auto erase(const cell_t<I> & cell) -> void
        requires(I > 0 && I <= N)
        {
            // erase the cell from the mesh
            std::get<I>(_cells).erase(cell);

            // TOFIX: is this the only reason that {_geometry} cannot be a const reference?
            // erase cell from topology
            _geometry.topology().erase(cell);

            // all done
            return;
        }

        /**
         * @brief Returns a mesh with all boundary cells of dimension I
         */
        template <int I = N - 1>
        inline auto boundary() -> Mesh<D, cellT, I>
        requires(I == N - 1)
        {
            // instantiate a new mesh for the boundary elements
            Mesh<D, cellT, I> boundary_mesh(_geometry);

            // loop on the (I+1) dimensional cells
            for (const auto & cell : cells<I + 1>()) {
                // loop on the subcells of {cell}
                for (const auto & subcell : cell->composition()) {
                    // if {subcell} does not have a counterpart in {topology} with opposite
                    // orientation
                    if (!_geometry.topology().exists_flipped(subcell)) {
                        // add {subcell} to the boundary mesh
                        boundary_mesh.insert(subcell);
                    }
                }
            }

            // return the boundary mesh
            return boundary_mesh;
        }

      public:
        template <int I>
        inline auto insert(const cell_t<I> & cell) -> void
        requires(I >= 0 && I <= N)
        {
            // add the cell to the set of cells with same dimension
            std::get<I>(_cells).insert(cell);

            // all done
            return;
        }

      private:
        // a reference to the geometry
        geometry_t & _geometry;

        // container to store N+1 containers of d dimensional cells with d = 0, ..., N
        cell_tuple_t _cells;
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
