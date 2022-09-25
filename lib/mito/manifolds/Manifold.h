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
        inline Manifold(const element_set_t<element_t> & elements) :
            _elements(elements.begin(), elements.end()),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        inline Manifold(element_set_t<element_t> && elements) :
            _elements(elements.begin(), elements.end()),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        inline Manifold(const element_vector_t<element_t> & elements) :
            _elements(elements),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        inline Manifold(element_vector_t<element_t> && elements) :
            _elements(elements),
            _jacobians(elements.size(), 0.0)
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

        inline auto elements() const -> const element_vector_t<element_t> & { return _elements; }
        inline auto nElements() const -> int { return _elements.size(); }
        inline auto jacobian(int e) const -> real { return _jacobians[e]; }
        inline auto coordinatesVertex(const vertex_t & v) const -> const auto &
        {
            return geometry::point_cloud<D>::point(v);
        }

      private:
        inline auto _computeJacobians() -> void
        {
            return mito::geometry::computeElementsVolume<
                element_t /* element type */, D /* spatial dim*/>(_elements, _jacobians);
        }

      private:
        const element_vector_t<element_t> _elements;
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
