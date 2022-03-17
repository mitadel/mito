// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    template <int D>
    class Mesh {

      private:
        // typedef for a container of oriented mesh simplices
        template <class T>
        using simplex_container = std::vector<T>;

        // typedef for a collection of oriented simplices of dimension I-1
        template <size_t I>
        using simplex_collection = simplex_container<oriented_simplex_t<int(I)> *>;

        // simplex_collection<I>... expands to:
        // simplex_container<oriented_simplex_t<0>*>, ..., simplex_container<oriented_simplex_t<D>*>
        template <typename = std::make_index_sequence<D>>
        struct simplices_tuple;

        template <size_t... I>
        struct simplices_tuple<std::index_sequence<I...>> {
            using type = std::tuple<simplex_container<simplex_t<0> *>, simplex_collection<I+1>...>;
        };

        // this expands to:
        // tuple<simplex_container<simplex_t<0>*>,
        //      simplex_container<oriented_simplex_t<1>*>, ...,
        //      simplex_container<oriented_simplex_t<D>*>
        using simplices_tuple_t = typename simplices_tuple<>::type;

      private:
        // typedef for a composition map of mesh simplices:
        // these maps map:
        //      2 pointers to nodes into a pointer to edge,
        //      3 pointers to edges into a pointer to face, ...
        // std::map<std::array<simplex_t<0> *, 2>, simplex_t<1> *>  edges composition
        // std::map<std::array<simplex_t<1> *, 3>, simplex_t<2> *>  faces compositions
        // std::map<std::array<simplex_t<2> *, 4>, simplex_t<3> *>  volumes compositions
        template <size_t I>
        using composition_map = std::map<
            std::array<simplex_t<int(I - 1)> *, I + 1>, std::shared_ptr<simplex_t<int(I)>>>;

        template <typename = std::make_index_sequence<D>>
        struct composition_tuple;

        template <size_t... I>
        struct composition_tuple<std::index_sequence<I...>> {
            using type = std::tuple<composition_map<I + 1>...>;
        };

        // this expands to:
        // tuple<composition_map<1>, ..., composition_map<D>
        using composition_tuple_t = typename composition_tuple<>::type;

      public:
        Mesh(std::string meshFileName) : _simplices(), _compositions(), _vertices()
        {
            _loadMesh(meshFileName);
        }

        template <size_t I>
        void _deleteSimplices()
        {
            // delete simplices of dimension I
            for (auto simplex : std::get<I>(_simplices)) {
                delete simplex;
            }

            // all done
            return;
        }

        template <size_t... I>
        void _deleteAllSimplices(std::index_sequence<I...>)
        {
            // delete simplices of dimension I for all I's in the index sequence
            ((_deleteSimplices<I>()), ...);

            // all done
            return;
        }

        ~Mesh()
        {
            // delete all simplices from dimension 0 (included) to D (included)
            _deleteAllSimplices(std::make_index_sequence<D + 1> {});

            // all done
            return;
        }

      private:
        // delete default constructor
        Mesh() = delete;

        // delete copy constructor
        Mesh(const Mesh &) = delete;

        // delete move constructor
        Mesh(const Mesh &&) = delete;

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
        const auto & elements() const requires (I <= D)
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
         * @brief Returns an element set with all simplices of dimension I 
         */
        template <int I>
        constexpr auto element_set() const requires(I <= D)
        {
            return mito::mesh::element_set(elements<I>(), vertices());
        }

      private:
        /**
         * @brief Registers a simplex in the composition map
         *
         * @tparam I dimension of the simplex to insert
         * @param simplex mesh simplex to be inserted
         * @return the pair returned by the map insertion:
         *          pair::first is an iterator pointing to either the newly inserted simplex or
         * to the simplex with an equivalent key pair::second is true (false) if the simplex was
         * inserted (was already in the map)
         */
        template <int I>
        auto _registerSimplexComposition(std::shared_ptr<simplex_t<I>> & simplex_p) requires (I <= D)
        {
            auto sorted_simplices = simplex_p.get()->simplices();
            std::sort(sorted_simplices.begin(), sorted_simplices.end());

            return std::get<I - 1>(_compositions)
                .insert(
                    std::pair<std::array<simplex_t<I - 1> *, I + 1>, std::shared_ptr<simplex_t<I>>>(
                        sorted_simplices, simplex_p));
        }

        template <int I>
        bool _computeSimplexOrientation(
            std::array<simplex_t<I - 1> *, I + 1> composition,
            const std::shared_ptr<simplex_t<I>>& simplex)
        {
            // auto first_simplex = std::min_element(composition.begin(), composition.end());
            // std::rotate(composition.begin(), first_simplex, composition.end());
            if (composition == simplex.get()->simplices()) {
                return true;
            }
            return false;
        }

        /**
         * @brief Adds a new composed simplex (i.e. edge, face, element) if it is not a repetition
         *         of an equivalent already registered composed simplex
         *
         * @tparam I dimension of the composed simplex to add (1, 2, ..., D)
         * @param composition simplex composition in terms of I + 1 simplices of dimension (I - 1)
         * @return simplex_t<I>* a pointer either to the newly added simplex or to the equivalent
         *                              already registered composed simplex
         */
        template <int I>
        auto _addUniqueSimplex(
            const std::array<simplex_t<I - 1> *, I + 1> & composition) requires(I <= D)
        {
            // instantiate new simplex with this composition
            auto simplex = std::make_shared<simplex_t<I>>(composition);
            // look up the new simplex with its composition and register it if it does not exist yet
            auto ret = _registerSimplexComposition(simplex);
            // if the simplex already existed
            if (ret.second == false) {
                // delete the new simplex (it was just a repeated entry)
                simplex.reset();
            }

            // if I == D then ret.second == true, that is there shall be no repetitions of the
            // elements of highest dimension
            assert((I != D) || ret.second);

            // return a pointer to the newly added simplex
            return ret.first->second;
        }

        template <int I>
        void _addSimplex(oriented_simplex_t<I> * simplex) requires(I <= D)
        {
            // TOFIX: is push_back expensive even when we reserve the space? No, but we only
            // know in advance how many nodes and elements are in the mesh, not how many edges
            // or faces, so we cannot reserve memory for edges and faces in advance...
            std::get<I>(_simplices).push_back(simplex);

            // all done
            return;
        }

        void _addVertex(point_t<D> && point)
        {
            // instantiate new vertex
            vertex_t * vertex = new vertex_t();
            // associate the new vertex to the new point
            _vertices.insert(*vertex, point);
            // add to the simplices the newly created vertex
            std::get<0>(_simplices).push_back(vertex);

            // all done
            return;
        }

        template <int I>
        auto _getSimplex(int n) requires(I <= D)
        {
            return std::get<I>(_simplices)[n];
        }

        void _readTriangle(std::ifstream & fileStream)
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

            vertex_t * vertex0 = _getSimplex<0>(index0);
            vertex_t * vertex1 = _getSimplex<0>(index1);
            vertex_t * vertex2 = _getSimplex<0>(index2);

            std::array<vertex_t *, 2> composition0 { vertex0, vertex1 };
            auto segment0 = _addUniqueSimplex<1>(composition0);
            bool orientation0 = _computeSimplexOrientation(composition0, segment0);
            // instantiate a new oriented simplex and give it shared ownership on its footprint
            oriented_simplex_t<1> * oriented_segment0 = new oriented_simplex_t<1>(segment0, 
                orientation0);
            // add the oriented simplex as a new one
            _addSimplex(oriented_segment0);

            std::array<vertex_t *, 2> composition1 { vertex1, vertex2 };
            auto segment1 = _addUniqueSimplex<1>(composition1);
            bool orientation1 = _computeSimplexOrientation(composition1, segment1);
            // instantiate a new oriented simplex and give it shared ownership on its footprint
            oriented_simplex_t<1> * oriented_segment1 =
                new oriented_simplex_t<1>(segment1, orientation1);
            // add the oriented simplex as a new one
            _addSimplex(oriented_segment1);

            std::array<vertex_t *, 2> composition2 { vertex2, vertex0 };
            auto segment2 = _addUniqueSimplex<1>(composition2);
            bool orientation2 = _computeSimplexOrientation(composition2, segment2);
            // instantiate a new oriented simplex and give it shared ownership on its footprint
            oriented_simplex_t<1> * oriented_segment2 =
                new oriented_simplex_t<1>(segment2, orientation2);
            // add the oriented simplex as a new one
            _addSimplex(oriented_segment2);

            // QUESTION: Can the label be more than one?
            // read label for element
            // TOFIX: Ignored for now
            std::string element_set_id;
            fileStream >> element_set_id;

            std::array<simplex_t<1> *, 3> composition { segment0.get(), segment1.get(), 
                segment2.get() };
            auto simplex = _addUniqueSimplex<2>(composition);
            // instantiate a new oriented simplex and give it shared ownership on its footprint
            oriented_simplex_t<2> * oriented_simplex =
                new oriented_simplex_t<2>(simplex, true);
            // add the oriented simplex as a new one
            _addSimplex(oriented_simplex);

            // all done
            return;
        }

        void _readVertices(std::ifstream & fileStream, int N_vertices)
        {
            // fill in vertices
            for (int n = 0; n < N_vertices; ++n) {
                // instantiate new point
                point_t<D> point;
                for (int d = 0; d < D; ++d) {
                    // read point coordinates
                    fileStream >> point[d];
                }
                _addVertex(std::move(point));
            }
            // _vertices.print();

            // all done
            return;
        }

        void _readElements(std::ifstream & fileStream, int N_elements)
        {
            for (int i = 0; i < N_elements; ++i) {
                int element_type = 0;
                fileStream >> element_type;

                if (element_type == 3) {
                    _readTriangle(fileStream);
                } else {
                    std::cout << "Error: Unknown element type" << std::endl;
                }
            }

            // all done
            return;
        }

        void _loadMesh(std::string meshFileName)
        {
            std::cout << "Loading mesh..." << std::endl;

            // open mesh file
            std::ifstream fileStream;
            fileStream.open(meshFileName);
            assert(fileStream.is_open());

            // read dimension of physical space
            int dim = 0;
            fileStream >> dim;

            // assert this mesh object is of same dimension of the mesh being read
            assert(int(D) == dim);

            // read number of vertices
            int N_vertices = 0;
            fileStream >> N_vertices;
            // reserve space for vertices
            std::get<0>(_simplices).reserve(N_vertices);

            // read number of elements
            int N_elements = 0;
            fileStream >> N_elements;
            // reserve space for elements
            std::get<D>(_simplices).reserve(N_elements);

            // read number of element sets
            int N_element_sets = 0;
            fileStream >> N_element_sets;

            // QUESTION: Not sure that we need this...
            assert(N_element_sets == 1);

            // read the vertices
            _readVertices(fileStream, N_vertices);

            // read the elements
            _readElements(fileStream, N_elements);

            // sanity check: the number of vertices in the map is N_vertices
            assert(nElements<0>() == N_vertices);

            // sanity check: the number of elements of highest dimension in the map is N_elements
            assert(nElements<D>() == N_elements);

            // sanity check: run sanity check for all mesh simplices in cascade
            assert(sanityCheck());

            // finalize file stream
            fileStream.close();

            // all done
            return;
        }

      private:
        // container to store D+1 containers of d dimensional simplices with d = 0, ..., D
        simplices_tuple_t _simplices;
        // container to store D maps with the composition of i-dimensional simplices in terms
        // of arrays of (i-1)-dimensional simplices
        composition_tuple_t _compositions;
        // the mesh vertices
        VertexSet<D> _vertices;
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
