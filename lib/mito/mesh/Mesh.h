// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    template <int D, template <int> class cellT>
    class Mesh {

      private:
        template <int I>
        using cell_t = cellT<I>;

        // typedef for a collection of cells of dimension I
        template <size_t I>
        using cell_collection = element_set_t<cell_t<I>>;

        // cell_collection<I>... expands to:
        // cell_set_t<cell_t<1>>, ..., cell_set_t<cell_t<D>>
        template <typename = std::make_index_sequence<D + 1>>
        struct cell_tuple;

        template <size_t... I>
        struct cell_tuple<std::index_sequence<I...>> {
            using type = std::tuple<cell_collection<I>...>;
        };

        // this expands to:
        // tuple<cell_set_t<cell_t<0>>,
        //      cell_set_t<cell_t<1>>, ...,
        //      cell_set_t<cell_t<D>>
        using cell_tuple_t = typename cell_tuple<>::type;

      public:
        // default constructor
        inline Mesh() : _cells(), _vertices() {};

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
            for (const auto & cell : std::get<D>(_cells)) {
                if (!cell->sanityCheck()) {
                    return false;
                }
            }

            return true;
        }

        template <int I>
        inline auto nCells() const -> int
        requires(I <= D)
        {
            // all done
            return std::get<I>(_cells).size();
        }

        template <int I>
        inline auto cells() const -> const auto & requires(I <= D) {
                                                      // all done
                                                      return std::get<I>(_cells);
                                                  }

        inline auto vertices() const -> const auto &
        {
            // all done
            return _vertices;
        }

        // TODO: accessor operator[](point_t) -> a list of all vertices sitting on the same point
        auto point(const vertex_t & vertex) -> const point_t<D> &
        {
            return _vertices.find(vertex)->second;
        }

        template <int I>
        inline auto erase(const cell_t<I> & cell) -> void
        requires(I > 0 && I <= D)
        {
            // erase the cell from the mesh
            std::get<I>(_cells).erase(cell);

            // all done
            return;
        }

      public:
        template <int I>
        inline auto insert(const cell_t<I> & cell) -> void
        requires(I > 0 && I <= D)
        {
            // add the cell to the set of cells with same dimension
            std::get<I>(_cells).insert(cell);

            // all done
            return;
        }

        inline auto insert(const vertex_t & vertex, const point_t<D> & point) -> void
        {
            // register the vertex - point relation with the mesh
            _vertices.insert(std::pair<vertex_t, point_t<D>>(vertex, point));
        }

      private:
        // container to store D+1 containers of d dimensional cells with d = 0, ..., D
        cell_tuple_t _cells;

        // the mapping of vertices to points
        vertex_point_table_t<D> _vertices;
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
