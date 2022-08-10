// code guard
#if !defined(mito_manifolds_Manifold_h)
#define mito_manifolds_Manifold_h


namespace mito::manifolds {

    // QUESTION:
    // In the case of a bulk element or a line element in 3D the differential in the
    // integral is different (dV, or |r'(d)| dt). Is it a good idea to implement the jacobian, the
    // derivative of the parametrization, the normal times the area differential, as the returned
    // value by a method called, say, differential?

    template <class elementT, int D>
    class Manifold {

      public:
        using element_t = elementT;
        static constexpr int dim = D;

      public:
        Manifold(
            const mesh::simplex_set_t<element_t> & elements) :
            _elements(elements.begin(), elements.end()),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        Manifold(
            mesh::simplex_set_t<element_t> && elements) :
            _elements(elements.begin(), elements.end()),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        Manifold(
            const mesh::simplex_vector_t<element_t> & elements) :
            _elements(elements),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        Manifold(
            mesh::simplex_vector_t<element_t> && elements) :
            _elements(elements),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        ~Manifold() {}

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
        bool sanityCheck()
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

        inline const mesh::simplex_vector_t<element_t> & elements() const { return _elements; }
        inline int nElements() const { return _elements.size(); }
        inline real jacobian(int e) const { return _jacobians[e]; }
        inline const auto & coordinatesVertex(const mesh::oriented_simplex_ptr<0> & v) const
        {
            return mesh::point_cloud<D>::point(v);
        }

      private:
        void _computeJacobians()
        {
            return computeElementsVolume<element_t /* element type */, D /* spatial dim*/>(
                _elements, _jacobians);
        }

      private:
        const mesh::simplex_vector_t<element_t> _elements;
        std::vector<real> _jacobians;
    };

    template <class elementT, int D>
    std::ostream & operator<<(std::ostream & os, const manifold_t<elementT, D> & manifold)
    {
        os << "Element set: " << std::endl;

        for (const auto e : manifold.elements()) {
            os << "Composition: " << std::endl;
            os << *e;
            os << "Vertices: " << std::endl;
            mesh::vertex_set_t vertices;
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
