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

    template <
        int I /* dimension of the manifold */, int D /* dimension of physical space */,
        template <int> class elementT /* manifold element type */>
    requires(I <= D)
    class Manifold {

      public:
        using element_t = elementT<I>;
        static constexpr int dim = I;

      public:
        inline Manifold(const mesh::mesh_t<D, elementT> & mesh) :
            _elements(mesh.template elements<I>().begin(), mesh.template elements<I>().end()),
            _vertices(mesh.vertices()),
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
            // TOFIX: add intermediate function
            // get the point corresponding to vertex {v}
            return _vertices.find(v)->second->coordinates();
        }

      private:
        inline auto _computeJacobians() -> void
        {
            return computeElementsVolume(_elements, _vertices, _jacobians);
        }

      private:
        // TOFIX: not sure I like that {_elements} is a copy while {_vertices} is a reference
        const element_vector_t<element_t> _elements;
        // QUESTION: should the manifold hold directly a reference to the mesh?
        // the mapping of vertices to points
        const mesh::vertex_point_table_t<D> & _vertices;
        std::vector<real> _jacobians;
    };

    template <int I, int D, template <int> class elementT>
    std::ostream & operator<<(std::ostream & os, const manifold_t<I, D, elementT> & manifold)
    {
        os << "Element set: " << std::endl;

        for (const auto e : manifold.elements()) {
            os << "Composition: " << std::endl;
            os << *e;
            os << "Vertices: " << std::endl;
            vertex_set_t vertices;
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
