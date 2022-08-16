// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    template <int D>
    class Mesh {

      private:
        template <class T>
        using vertex_container = std::vector<T>;

        // QUESTION: would it be better to use reference wrappers here?
        // typedef for a collection of oriented simplices of dimension I
        template <size_t I>
        using simplex_collection = simplex_set_t<oriented_simplex_t<int(I)>>;

        // simplex_collection<I>... expands to:
        // simplex_set_t<oriented_simplex_t<1>>, ..., simplex_set_t<oriented_simplex_t<D>>
        template <typename = std::make_index_sequence<D>>
        struct simplices_tuple;

        template <size_t... I>
        struct simplices_tuple<std::index_sequence<I...>> {
            using type =
                std::tuple<vertex_container<oriented_simplex_ptr<0>>, simplex_collection<I + 1>...>;
        };

        // this expands to:
        // tuple<simplex_set_t<simplex_t<0>>,
        //      simplex_set_t<oriented_simplex_t<1>>, ...,
        //      simplex_set_t<oriented_simplex_t<D>>
        using simplices_tuple_t = typename simplices_tuple<>::type;

      public:
        // default constructor
        Mesh() : _simplices(), _vertices() {};

        ~Mesh() {}

        // move constructor
        Mesh(Mesh &&) = default;

      private:
        // delete copy constructor
        Mesh(const Mesh &) = delete;

        // delete assignment operator
        const Mesh & operator=(const Mesh &) = delete;

        // delete move assignment operator
        const Mesh & operator=(const Mesh &&) = delete;

      public:
        bool sanityCheck()
        {
#if 0
            // print summary
            std::cout << "Mesh composition: " << std::endl;
            std::cout << "0: " << std::get<0>(_simplices).size() << " simplices " << std::endl;
            std::cout << "1: " << std::get<1>(_simplices).size() << " simplices " << std::endl;
            std::cout << "2: " << std::get<2>(_simplices).size() << " simplices " << std::endl;
#endif

            // sanity check: each simplex is self-consistent
            for (const auto & simplex : std::get<D>(_simplices)) {
                if (!simplex->sanityCheck()) {
                    return false;
                }
            }

            return true;
        }

        template <int I>
        int nElements() const requires(I <= D)
        {
            // all done
            return std::get<I>(_simplices).size();
        }

        template <int I>
        auto & elements() requires(I <= D)
        {
            // all done
            return std::get<I>(_simplices);
        }

        const auto & vertices() const
        {
            // all done
            return _vertices;
        }

        /**
         * @brief Returns an element set with all boundary simplices of dimension I
         */
        // QUESTION: I don't like the asymmetry of elements returning a const reference and boundary
        //  elements returning an instance. Either:
        //  1) say that these methods will make copies of the elements for the client to use, or
        //  2) say that boundary_elements will create a new data structure at run time and return a
        //      (const) reference for the client to use.
        template <int I>
        constexpr auto boundary_elements() const requires(I<D && I> 0)
        {
            // instantiate a simplex collection
            simplex_collection<I> boundary_simplices;

            // loop on simplices (D-1) dimensional simplices
            for (const auto & simplex : std::get<D - 1>(_simplices)) {
                // if the simplex footprint has only one occurrence then it is on the boundary
                if (!exists_flipped(simplex)) {
                    // add the subsimplices of dimension I to the set of boundary simplices
                    simplex->template getSimplices<I>(boundary_simplices);
                }
            }

            // return the boundary simplices
            return boundary_simplices;
        }

        template <int I>
        void _erase(const oriented_simplex_ptr<I> & simplex) requires(I > 0 && I <= D)
        {
            // erase the subsimplices from the mesh (erase bottom -> up)
            for (const auto & subsimplex : simplex->simplices()) {
                std::get<I - 1>(_simplices).erase(subsimplex);
            }

            // erase the simplex from the mesh
            std::get<I>(_simplices).erase(simplex);

            // all done
            return;
        }

        template <int I>
        void _erase(const oriented_simplex_ptr<I> & simplex) requires(I == 0)
        {
            // all done
            return;
        }

        template <int I>
        void erase(const oriented_simplex_ptr<I> & simplex) requires(I > 0 && I <= D)
        {
            // QUESTION: can we wrap simplices in a way that the reference count can be called
            //  incidence?

            // erase recursively until D = 0
            _erase(simplex);

            // cleanup oriented simplex factory around this simplex
            OrientedSimplexFactory<I>::cleanup(simplex);

            // all done
            return;
        }

      public:
        template <int I>
        void addSimplex(const oriented_simplex_ptr<I> & simplex) requires(I > 0 && I <= D)
        {
            // add the oriented simplex to the set of simplices with same dimension
            std::get<I>(_simplices).insert(simplex);

            // all done
            return;
        }

        void addVertex(point_t<D> && point)
        {
            // instantiate new vertex
            auto vertex = mito::mesh::vertex();
            // associate the new vertex to the new point
            _vertices.insert(vertex, point);
            // add to the simplices the newly created vertex
            std::get<0>(_simplices).push_back(vertex);

            // all done
            return;
        }

        auto & getVertex(int n)
        {
            return std::get<0>(_simplices)[n];
        }

      private:
        // container to store D+1 containers of d dimensional simplices with d = 0, ..., D
        simplices_tuple_t _simplices;
        // the mesh vertices
        point_cloud_t<D> _vertices;
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
