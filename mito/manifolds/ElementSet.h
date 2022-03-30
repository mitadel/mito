// code guard
#if !defined(mito_manifolds_ElementSet_h)
#define mito_manifolds_ElementSet_h


namespace mito::manifolds {

    template <int D>
    real computeDistance(
        const mito::mesh::point_t<D> & pointA, const mito::mesh::point_t<D> & pointB)
    {
        // return the distance between the two points
        auto dist = pointA - pointB;
        return sqrt(dist * dist);
    }

    template <int D>
    void computeSimplicesVolume(
        const std::vector<mito::mesh::oriented_simplex_t<D> *> & elements,
        const mito::mesh::point_cloud_t<D> & points, std::vector<real> & volumes)
    {
        // number of element vertices
        constexpr int V = D + 1;

        // a container to store the coordinates of each vertex in a tensor
        static matrix_t<V> pointsTensor;

        // assert memory allocation is consistent
        assert(volumes.size() == elements.size());

        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // reinitialize pointsTensor for a new element
            pointsTensor.reset();

            // use a set to collect vertices without repeated entries
            std::unordered_set<mito::mesh::vertex_t *> element_vertices;
            element->vertices(element_vertices);
            // assert you found V element vertices
            assert(V == element_vertices.size());

            // loop on element vertices
            int v = 0;
            for (const auto & vertex : element_vertices) {
                // fill up pointsTensor container
                for (int d = 0; d < D; ++d) {
                    pointsTensor[v * V + d] = points[vertex][d];
                }
                pointsTensor[v * V + D] = 1.0;
                // update element vertices counter
                ++v;
            }

            // compute the volume of the e-th element
            volumes[e] = fabs(pyre::algebra::determinant(pointsTensor)) 
                / pyre::algebra::factorial<D>();
            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <class element_t, int D>
    void computeElementsVolume(
        const std::vector<element_t *> & elements, const mito::mesh::point_cloud_t<D> & points,
        std::vector<real> & volumes);

    template <>
    void computeElementsVolume<mito::mesh::triangle_t, 2>(
        const std::vector<mito::mesh::triangle_t *> & elements,
        const mito::mesh::point_cloud_t<2> & points, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<2>(elements, points, volumes);
    }

    template <>
    void computeElementsVolume<mito::mesh::tetrahedron_t, 3>(
        const std::vector<mito::mesh::tetrahedron_t *> & elements,
        const mito::mesh::point_cloud_t<3> & points, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<3>(elements, points, volumes);
    }

    template <>
    void computeElementsVolume<mito::mesh::segment_t, 1>(
        const std::vector<mito::mesh::segment_t *> & elements,
        const mito::mesh::point_cloud_t<1> & points, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<1>(elements, points, volumes);
    }

    template <int D>
    void computeSegmentsLength(
        const std::vector<mito::mesh::segment_t *> & elements,
        const mito::mesh::point_cloud_t<D> & points, std::vector<real> & length)
    {
        // number of element vertices
        constexpr int V = 2;

        // assert memory allocation is consistent
        assert(length.size() == elements.size());

        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // collect vertices
            std::unordered_set<mito::mesh::vertex_t *> vertices_set;
            element->vertices(vertices_set);
            std::vector<const mito::mesh::vertex_t *> element_vertices(
                vertices_set.begin(), vertices_set.end());

            // assert the size of vertices container is equal to the number of element vertices
            assert(element_vertices.size() == V);

            // store the distance between the two element vertices as the element length
            length[e] = computeDistance<D>(
                points[element_vertices[0]], points[element_vertices[1]]);

            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <>
    void computeElementsVolume<mito::mesh::segment_t, 2>(
        const std::vector<mito::mesh::segment_t *> & elements,
        const mito::mesh::point_cloud_t<2> & points, std::vector<real> & volumes)
    {
        return computeSegmentsLength<2>(elements, points, volumes);
    }

    template <>
    void computeElementsVolume<mito::mesh::segment_t, 3>(
        const std::vector<mito::mesh::segment_t *> & elements,
        const mito::mesh::point_cloud_t<3> & points, std::vector<real> & volumes)
    {
        return computeSegmentsLength<3>(elements, points, volumes);
    }

    // follows implementation by Kahan2014
    template <int D = 3>
    void computeTriangleArea(
        const std::vector<mito::mesh::triangle_t *> & elements,
        const mito::mesh::point_cloud_t<D> & points, std::vector<real> & areas)
    {
        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // collect vertices
            std::unordered_set<mito::mesh::vertex_t *> vertices_set;
            element->vertices(vertices_set);
            std::vector<const mito::mesh::vertex_t *> element_vertices(
                vertices_set.begin(), vertices_set.end());

            // compute lengths of three edges
            std::array<real, 3> edges_lengths;
            edges_lengths[0] = computeDistance<D>(
                points[element_vertices[0]], points[element_vertices[1]]);
            edges_lengths[1] = computeDistance<D>(
                points[element_vertices[0]], points[element_vertices[2]]);
            edges_lengths[2] = computeDistance<D>(
                points[element_vertices[1]], points[element_vertices[2]]);

            // sort edges lengths in ascending order
            std::sort(edges_lengths.begin(), edges_lengths.end());

            // a >= b >= c
            real a = edges_lengths[2];
            real b = edges_lengths[1];
            real c = edges_lengths[0];
            real amb = a - b;
            real bmc = b - c;
            real bpc = b + c;

            // compute area of element e
            areas[e] = 0.25 * sqrt((a + bpc) * (c - amb) * (c + amb) * (a + bmc));

            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <>
    void computeElementsVolume<mito::mesh::triangle_t, 3>(
        const std::vector<mito::mesh::triangle_t *> & elements,
        const mito::mesh::point_cloud_t<3> & points, std::vector<real> & volumes)
    {
        return computeTriangleArea(elements, points, volumes);
    }

    // QUESTION:
    // In the case of a bulk element or a line element in 3D the differential in the
    // integral is different (dV, or |r'(d)| dt). Is it a good idea to implement the jacobian, the
    // derivative of the parametrization, the normal times the area differential, as the returned
    // value by a method called, say, differential?

    template <class elementT, int D>
    class ElementSet {

      public:
        using element_t = elementT;
        static constexpr int dim = D;

      public:
        ElementSet(
            const std::unordered_set<element_t *> & elements,
            const mito::mesh::point_cloud_t<D> & points) :
            _elements(elements.begin(), elements.end()),
            _points(points),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        ElementSet(
            std::unordered_set<element_t *> && elements,
            const mito::mesh::point_cloud_t<D> & points) :
            _elements(elements.begin(), elements.end()),
            _points(points),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        ElementSet(
            const std::vector<element_t *> & elements,
            const mito::mesh::point_cloud_t<D> & points) :
            _elements(elements),
            _points(points),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        ElementSet(
            std::vector<element_t *> && elements, const mito::mesh::point_cloud_t<D> & points) :
            _elements(elements),
            _points(points),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        ElementSet(
            const std::vector<element_t *> & elements,
            const mito::mesh::point_cloud_t<D> && points) = delete;

        ElementSet(
            std::vector<element_t *> && elements,
            const mito::mesh::point_cloud_t<D> && points) = delete;

        ~ElementSet() {}

      private:
        // delete default constructor
        ElementSet() = delete;

        // delete copy constructor
        ElementSet(const ElementSet &) = delete;

        // delete move constructor
        ElementSet(const ElementSet &&) = delete;

        // delete assignment operator
        const ElementSet & operator=(const ElementSet &) = delete;

        // delete move assignment operator
        const ElementSet & operator=(const ElementSet &&) = delete;

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

        inline const std::vector<element_t *> & elements() const { return _elements; }
        inline int nElements() const { return _elements.size(); }
        inline int nVertices() const { return element_t::nVertices(); }
        inline real jacobian(int e) const { return _jacobians[e]; }
        inline const auto & coordinatesVertex(const mito::mesh::vertex_t * v) const
        {
            return _points[v];
        }

      private:
        void _computeJacobians()
        {
            return computeElementsVolume<element_t /* element type */, D /* spatial dim*/>(
                _elements, _points, _jacobians);
        }

      private:
        const std::vector<element_t *> _elements;
        const mito::mesh::point_cloud_t<D> & _points;
        std::vector<real> _jacobians;
    };

    template <class elementT, int D>
    std::ostream & operator<<(std::ostream & os, const element_set_t<elementT, D> & element_set)
    {
        os << "Element set: " << std::endl;

        for (const auto e : element_set.elements()) {
            os << "Composition: " << std::endl;
            os << *e;
            os << "Vertices: " << std::endl;
            std::unordered_set<mito::mesh::vertex_t *> vertices;
            e->vertices(vertices);
            for (const auto v : vertices) {
                os << element_set.coordinatesVertex(v) << std::endl;
            }
            os << std::endl;
        }

        return os;
    }

}    // namespace mito


#endif

// end of file
