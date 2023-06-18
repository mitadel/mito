// code guard
#if !defined(mito_manifolds_Manifold_h)
#define mito_manifolds_Manifold_h


namespace mito::manifolds {

    // QUESTION:
    // In the case of a bulk element or a line element in 3D the differential in the
    // integral is different (dV, or |r'(d)| dt). Is it a good idea to implement the jacobian, the
    // derivative of the parametrization, the normal times the area differential, as the returned
    // value by a method called, say, differential?

    // TOFIX: distinguish between element family/class (simplicial) and element type (triangles)

    template <class cellT /* the type of cell */, int D /* spatial dimension */>
    class Manifold {

      public:
        // typedef for cell type
        using cell_t = cellT;
        // get the order of the cell
        static constexpr int N = cellT::resource_t::order;
        // a point in parametric coordinates
        static constexpr int parametricDim = parametric_dim<cell_t>();
        using parametric_point_t = manifolds::parametric_point_t<parametricDim>;
        // the dimension of the physical space
        static constexpr int dim = D;
        // typedef for mesh type
        using mesh_t = mesh::mesh_t<cell_t, D>;
        // typedef for vertex
        using vertex_t = topology::vertex_t;

      public:
        inline Manifold(mesh_t & mesh) :
            _elements(mesh.cells().begin(), mesh.cells().end()),
            _vertices(mesh.geometry().nodes()),
            _jacobians(std::size(_elements), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        inline ~Manifold() {}

      private:
        // delete default constructor
        Manifold() = delete;

        // delete copy constructor
        Manifold(const Manifold &) = delete;

        // delete move constructor
        Manifold(Manifold &&) = delete;

        // delete assignment operator
        Manifold & operator=(const Manifold &) = delete;

        // delete move assignment operator
        Manifold & operator=(Manifold &&) = delete;

      public:
        inline auto sanityCheck() -> bool
        {
            bool check = true;
            for (const auto & e : _elements) {
                if (!e->sanityCheck()) {
                    std::cout << "Failed sanity check for element " << e << std::endl;
                    check = false;
                }
            }
            return check;
        }

        inline auto elements() const -> const element_vector_t<cell_t> & { return _elements; }
        inline auto nElements() const -> int { return std::size(_elements); }
        inline auto jacobian(int e) const -> real { return _jacobians[e]; }
        inline auto coordinatesVertex(const vertex_t & v) const -> const vector_t<D> &
        {
            // get the coordinates of the point attached to vertex {v}
            return _point(v)->coordinates();
        }

        inline auto parametrization(const cell_t & cell, const parametric_point_t & point) const
            -> vector_t<D>
        {
            // use a set to collect vertices without repeated entries
            topology::vertex_set_t vertices;
            cell->vertices(vertices);
            vector_t<D> coordinates;
            // loop on vertices
            int v = 0;
            for (const auto & vertex : vertices) {
                const auto & vertexCoordinates = coordinatesVertex(vertex);
                coordinates += point[v] * vertexCoordinates;

                ++v;
            }

            return coordinates;
        }

        inline auto print() const -> void
        {
            // print the element set of the manifold
            std::cout << "Element set: " << std::endl;

            for (const auto & e : elements()) {
                // print the elemental composition
                std::cout << "Composition: " << std::endl;
                std::cout << e;
                // and the coordinates of the vertices
                std::cout << "Vertices: " << std::endl;
                topology::vertex_set_t vertices;
                e->vertices(vertices);
                for (const auto & v : vertices) {
                    std::cout << coordinatesVertex(v) << std::endl;
                }
                std::cout << std::endl;
            }
        }

      private:
        inline auto _point(const vertex_t & v) const -> const geometry::point_t<D> &
        {
            // look up the point attached to vertex {v}
            return _vertices.find(v)->second;
        }

        inline auto _computeJacobians() -> void
        {
            return computeElementsVolume(_elements, _vertices, _jacobians);
        }

      private:
        // TOFIX: not sure I like that {_elements} is a copy while {_vertices} is a reference
        const element_vector_t<cell_t> _elements;
        // QUESTION: should the manifold hold directly a reference to the mesh?
        // the mapping of vertices to points
        const geometry::nodes_t<D> & _vertices;
        std::vector<real> _jacobians;
    };

    template <class cellT, int D>
    std::ostream & operator<<(std::ostream & os, const manifold_t<cellT, D> & manifold)
    {
        // print the manifold
        manifold.print();

        // all done
        return os;
    }

}    // namespace mito


#endif

// end of file
