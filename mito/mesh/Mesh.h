// code guard
#if !defined(mito_mesh_Mesh_h)
#define mito_mesh_Mesh_h


namespace mito::mesh {

    // TOFIX: either call this SimplicialMesh or remove simplex assumption
    template <int D>
    class Mesh {

      private:

        // QUESTION: would it be better to use reference wrappers here?
        // typedef for a collection of oriented simplices of dimension I
        template <size_t I>
        using simplex_collection = simplex_set_t<oriented_simplex_t<int(I)>>;

        // simplex_collection<I>... expands to:
        // simplex_set_t<oriented_simplex_t<1>>, ..., simplex_set_t<oriented_simplex_t<D>>
        template <typename = std::make_index_sequence<D+1>>
        struct simplices_tuple;

        template <size_t... I>
        struct simplices_tuple<std::index_sequence<I...>> {
            using type = std::tuple<simplex_collection<I>... > ;
        };

        // this expands to:
        // tuple<simplex_set_t<simplex_t<0>>,
        //      simplex_set_t<oriented_simplex_t<1>>, ...,
        //      simplex_set_t<oriented_simplex_t<D>>
        using simplices_tuple_t = typename simplices_tuple<>::type;

      public:
        Mesh(std::string meshFileName) : _simplices()
        {
            _loadMesh(meshFileName);
        }

        ~Mesh() {}

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

            // loop on simplices (I+1) dimensional simplices
            for (const auto & simplex : std::get<I+1>(_simplices)) {
                for (const auto & subsimplex : simplex->simplices()) {
                    // if the simplex footprint has only one occurrence then it is on the boundary
                    if (!exists_flipped(subsimplex)) {
                        // add this (D-1) dimensional simplex to the set of boundary simplices
                        boundary_simplices.insert(subsimplex);
                    }
                }
            }

            // return the boundary simplices
            return boundary_simplices;
        }

        template <int I>
        void _erase(const oriented_simplex_ptr<I> & simplex) requires(I > 0 && I <= D)
        {
            // erase the subsimplices from the mesh (erase bottom -> up)
            for (const auto & subsimplex :
                 simplex->simplices()) {
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

        template <int I>
        void addSimplex(const oriented_simplex_ptr<I> & simplex) requires(I > 0 && I <= D)
        {
            // add the oriented simplex to the set of simplices with same dimension
            std::get<I>(_simplices).insert(simplex);

            // all done
            return;
        }

      private:
        void _readTriangle(
            std::ifstream & fileStream, const std::vector<oriented_simplex_ptr<0>> & vertices)
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

            auto vertex0 = vertices[index0];
            auto vertex1 = vertices[index1];
            auto vertex2 = vertices[index2];

            auto segment0 = segment({ vertex0, vertex1 });
            auto segment1 = segment({ vertex1, vertex2 });
            auto segment2 = segment({ vertex2, vertex0 });

            auto element = triangle({ segment0, segment1, segment2 });
            addSimplex(element);

            // QUESTION: Can the label be more than one?
            // read label for element
            // TOFIX: Ignored for now
            std::string element_label;
            fileStream >> element_label;

            // all done
            return;
        }

        void _readVertices(
            std::ifstream & fileStream, int N_vertices,
            std::vector<oriented_simplex_ptr<0>> & vertices)
        {
            // fill in vertices
            for (int n = 0; n < N_vertices; ++n) {
                // instantiate new point
                point_t<D> point;
                for (int d = 0; d < D; ++d) {
                    // read point coordinates
                    fileStream >> point[d];
                }

                // instantiate new vertex
                auto vertex = mesh::vertex(std::move(point));

                // instantiate new vertex and add it to {vertices}
                vertices.push_back(vertex);
            }

            // all done
            return;
        }

        void _readElements(
            std::ifstream & fileStream, int N_elements,
            const std::vector<oriented_simplex_ptr<0>> & vertices)
        {
            for (int i = 0; i < N_elements; ++i) {
                int element_type = 0;
                fileStream >> element_type;

                if (element_type == 3) {
                    _readTriangle(fileStream, vertices);
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
            std::vector<oriented_simplex_ptr<0>> vertices;
            vertices.reserve(N_vertices);

            // read number of elements
            int N_elements = 0;
            fileStream >> N_elements;
            // reserve space for elements
            std::get<D>(_simplices).reserve(N_elements);

            // read number of element types
            int N_element_types = 0;
            fileStream >> N_element_types;

            // QUESTION: Not sure that we need this...
            assert(N_element_types == 1);

            // read the vertices
            _readVertices(fileStream, N_vertices, vertices);

            // read the elements
            _readElements(fileStream, N_elements, vertices);

            // sanity check: the number of vertices in the map is N_vertices
            vertices.shrink_to_fit();
            assert(vertices.size() == N_vertices);

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
    };

}    // namespace mito


#endif    // mito_mesh_Mesh_h

// end of file
