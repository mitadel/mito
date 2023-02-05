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
        // the dimension of the physical space
        static constexpr int dim = D;
        // typedef for mesh type
        using mesh_t = mito::mesh::mesh_t<cell_t, D>;
        // typedef for vertex
        using vertex_t = mito::topology::vertex_t;

      public:
        inline Manifold(mesh_t & mesh) :
            _elements(mesh.cells().begin(), mesh.cells().end()),
            _vertices(mesh.geometry().nodes()),
            _jacobians(_elements.size(), 0.0)
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
        Manifold(const Manifold &&) = delete;

        // delete assignment operator
        const Manifold & operator=(const Manifold &) = delete;

        // delete move assignment operator
        const Manifold & operator=(const Manifold &&) = delete;

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

        inline auto elements() const -> const auto & { return _elements; }
        inline auto nElements() const -> int { return _elements.size(); }
        inline auto jacobian(int e) const -> real { return _jacobians[e]; }
        inline auto coordinatesVertex(const vertex_t & v) const -> const auto &
        {
            // get the coordinates of the point attached to vertex {v}
            return _point(v)->coordinates();
        }

      private:
        inline auto _point(const vertex_t & v) const -> const auto &
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
        os << "Element set: " << std::endl;

        for (const auto e : manifold.elements()) {
            os << "Composition: " << std::endl;
            os << *e;
            os << "Vertices: " << std::endl;
            topology::vertex_set_t vertices;
            e->vertices(vertices);
            for (const auto v : vertices) {
                os << manifold.coordinatesVertex(v) << std::endl;
            }
            os << std::endl;
        }

        return os;
    }

}    // namespace mito


#endif

// end of file
