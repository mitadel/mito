#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"
#include "../mesh/simplex.h"

namespace mito {

    template <dim_t D>
    void computeSimplicesVolume(
        const std::vector<Simplex<D> *> & elements, const VertexCoordinatesMap<D> & coordinatesMap,
        std::vector<real> & volumes)
    {
        // number of vertices
        constexpr int V = int(D) + 1;

        // a container to store the coordinates of each vertex in a tensor
        static tensor<mito::dim_t(V)> verticesTensor;

        // assert memory allocation is consistent
        assert(volumes.size() == elements.size());

        // loop on elements
        int e = 0;
        for (const auto & element : elements) {

            // reinitialize verticesTensor for a new element
            std::fill(verticesTensor.begin(), verticesTensor.end(), 0.0);

            // use a set to collect vertices without repeated entries
            std::set<const vertex_t *> vertices;
            element->getVertices(vertices);
            // assert you found D+1 vertices
            assert(V == vertices.size());

            // loop on vertices
            int v = 0;
            for (const auto & vertex : vertices) {
                // fill up verticesTensor container
                for (auto d = 0; d < D; ++d) {
                    verticesTensor[v * V + d] = coordinatesMap[vertex][d];
                }
                verticesTensor[v * V + D] = 1.0;
                // update vertices counter
                ++v;
            }

            // compute the volume of the e-th element
            volumes[e] = fabs(ComputeDeterminant(verticesTensor)) / Factorial<D>();
            // update elements counter
            ++e;
        }

        // all done
        return;
    }

    template <class element_t, dim_t D>
    void computeElementsVolume(
        const std::vector<element_t *> & elements, const VertexCoordinatesMap<D> & coordinatesMap,
        std::vector<real> & volumes);

    template <>
    void computeElementsVolume<triangle_t, DIM2>(
        const std::vector<triangle_t *> & elements,
        const VertexCoordinatesMap<DIM2> & coordinatesMap, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<DIM2>(elements, coordinatesMap, volumes);
    }

    template <>
    void computeElementsVolume<tetrahedron_t, DIM3>(
        const std::vector<tetrahedron_t *> & elements,
        const VertexCoordinatesMap<DIM3> & coordinatesMap, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<DIM3>(elements, coordinatesMap, volumes);
    }

    // QUESTION:
    // In the case of a bulk element or a line element in 3D the differential in the
    // integral is different (dV, or |r'(d)| dt). Is it a good idea to implement the jacobian, the
    // derivative of the parametrization, the normal times the area differential, as the returned
    // value by a method called, say, differential?

    template <class element_t, dim_t D>
    class ElementSet {

      public:
        ElementSet(
            std::vector<element_t *> && elements, const VertexCoordinatesMap<D> & coordinatesMap) :
            _elements(elements),
            _coordinatesMap(coordinatesMap),
            _jacobians(elements.size(), 0.0)
        {
            // compute the jacobians of the map from reference to current element for each element
            _computeJacobians();
        }

        ~ElementSet() {}

        // delete default constructor
        ElementSet() = delete;

        // delete copy constructor
        ElementSet(const ElementSet &) = delete;

        // delete assignment operator
        ElementSet & operator=(const ElementSet &) = delete;

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
        inline auto coordinatesVertex(const vertex_t * v) const { return _coordinatesMap[v]; }

      private:
        void _computeJacobians()
        {
            return computeElementsVolume<element_t /* element type */, D /* spatial dim*/>(
                _elements, _coordinatesMap, _jacobians);
        }

      private:
        const std::vector<element_t *> _elements;
        const VertexCoordinatesMap<D> & _coordinatesMap;
        std::vector<real> _jacobians;
    };

}    // namespace mito

#endif    //__MITO__ELEMENTS__

// end of file
