#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"
#include "nodal_field.h"

namespace mito {

    /**
     *  Structs for the element type
     */

    struct SEG {
        static constexpr auto name = "SEG";
        static constexpr DIM parametricDim = DIM2;
        static constexpr int nVertices = 2;
    };

    struct TRI {
        static constexpr auto name = "TRI";
        static constexpr DIM parametricDim = DIM3;
        static constexpr int nVertices = 3;
    };

    struct TET {
        static constexpr auto name = "TET";
        static constexpr DIM parametricDim = DIM4;
        static constexpr int nVertices = 4;
    };

    // template with respect to N, number of nodes per element
    template <int N>
    class Connectivity {
    public:
        Connectivity(int nel) : _nel(nel), _connectivityArray(nel * N, 0) {}
        Connectivity(int nel, std::vector<int> && connectivityArray) :
            _nel(nel),
            _connectivityArray(connectivityArray)
        {
            // assert that vector's dimension is compatible with nel and nen
            assert(_connectivityArray.size() == _nel * N);
            // all done
            return;
        }

        ~Connectivity() {}

        inline const int & operator()(int e, int a) const
        {
            assert(e < _nel && a < N);
            return _connectivityArray[e * N + a];
        }

        inline int & operator()(int e, int a)
        {
            assert(e < _nel && a < N);
            return _connectivityArray[e * N + a];
        }

        inline const int & operator()(int e) const
        {
            assert(e < _nel);
            return _connectivityArray[e * N];
        }

        inline int & operator()(int e)
        {
            assert(e < _nel);
            return _connectivityArray[e * N];
        }

        inline const std::vector<int> & operator()() const { return _connectivityArray; }

        inline std::vector<int> & operator()() { return _connectivityArray; }

        inline int nElements() const { return _nel; }

    private:
        // number of elements
        int _nel;
        // connectivity array
        std::vector<int> _connectivityArray;
    };

    template <DIM D>
    void computeSimplicesVolume(
        const std::vector<mito::vector<D>> & vertices, std::vector<real> & volumes)
    {
        // number of vertices
        constexpr int V = int(D) + 1;

        static tensor<mito::DIM(V)> verticesTensor;

        // get number of elements
        int nElements = volumes.size();

        // TOFIX: I am not supposed to know here the memory layout of vertices
        for (auto e = 0; e < nElements; ++e) {
            std::fill(verticesTensor.begin(), verticesTensor.end(), 0.0);
            for (auto a = 0; a < V; ++a) {
                for (auto d = 0; d < D; ++d) {
                    verticesTensor[a * V + d] = vertices[e * V + a][d];
                }
                verticesTensor[a * V + D] = 1.0;
            }
            volumes[e] = fabs(ComputeDeterminant(verticesTensor)) / Factorial<D>();
        }

        // all done
        return;
    }

    template <DIM D>
    void computeSegmentsLength(
        const std::vector<mito::vector<D>> & vertices, std::vector<real> & lengths)
    {
        // number of vertices
        constexpr int V = 2;

        // get number of elements
        int nElements = lengths.size();

        // TOFIX: I am not supposed to know here the memory layout of vertices
        for (auto e = 0; e < nElements; ++e) {
            real dist = 0.0;
            for (auto d = 0; d < D; ++d) {
                real dist_d = vertices[e * V + 0][d] - vertices[e * V + 1][d];
                dist = dist_d * dist_d;
            }
            lengths[e] = sqrt(dist);
        }
        // all done
        return;
    }

    template <int V, DIM D>
    void computeElementsVolume(
        const std::vector<mito::vector<D>> & vertices, std::vector<real> & volumes);

    template <>
    void computeElementsVolume<4, DIM3>(
        const std::vector<mito::vector<DIM3>> & vertices, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<DIM3>(vertices, volumes);
    }

    template <>
    void computeElementsVolume<3, DIM2>(
        const std::vector<mito::vector<DIM2>> & vertices, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<DIM2>(vertices, volumes);
    }

    template <>
    void computeElementsVolume<2, DIM2>(
        const std::vector<mito::vector<DIM2>> & vertices, std::vector<real> & volumes)
    {
        return computeSegmentsLength<DIM2>(vertices, volumes);
    }

    template <>
    void computeElementsVolume<2, DIM3>(
        const std::vector<mito::vector<DIM3>> & vertices, std::vector<real> & volumes)
    {
        return computeSegmentsLength<DIM3>(vertices, volumes);
    }

    template <>
    void computeElementsVolume<2, DIM1>(
        const std::vector<mito::vector<DIM1>> & vertices, std::vector<real> & volumes)
    {
        return computeSimplicesVolume<DIM1>(vertices, volumes);
    }

    // Collection of homogeneous elements
    template <class ElementType, DIM D>
    class Elements {

        static constexpr int V = ElementType::nVertices;

    private:
        template <int N>
        void _fillVertices(
            const Connectivity<N> & connectivity, const NodalField<real, D> & coordinates)
        {

            for (auto e = 0; e < _nElements; ++e) {
                for (auto a = 0; a < V; ++a) {
                    // TOFIX: Here we assume that the vertices are the first nVertices entries of
                    //       the connectivity...
                    const int & id = connectivity(e, a);
                    for (auto d = 0; d < D; ++d) {
                        vertex(e, a)[d] = coordinates(id, d);
                    }
                }
            }

            // all done
            return;
        }

        // computes volume of a simplex of vertices a1, ..., aV based on the formula
        // fabs(det(a1, 1; ...; aV, 1)) / D!, with D = V - 1 being the dimension of the phyisical
        // space
        void _computeJacobians()
        {

            computeElementsVolume<V /* nVertices */, D /* spatial dim*/>(_vertices, _jacobians);

            // all done
            return;
        }

    public:
        template <int N>
        Elements(const Connectivity<N> & connectivity, const NodalField<real, D> & coordinates) :
            _nElements(connectivity.nElements()),
            _vertices(_nElements * V),
            _jacobians(_nElements)
        {

            // fill container with elements vertices based on connectivity and coordinates
            _fillVertices(connectivity, coordinates);

            // compute the volume of each element
            _computeJacobians();

            // all done
            return;
        }

        ~Elements() {}

        const mito::vector<D> & vertex(int e, int v) const
        {
            assert(e < _nElements && v < V);
            return _vertices[e * V + v];
        }

        mito::vector<D> & vertex(int e, int v)
        {
            assert(e < _nElements && v < V);
            return _vertices[e * V + v];
        }

        inline int nElements() const { return _nElements; }
        inline int nVertices() const { return V; }
        inline real jacobian(int e) const { return _jacobians[e]; }

    private:
        int _nElements;
        std::vector<mito::vector<D>> _vertices;
        std::vector<real> _jacobians;
    };

    // template with respect to element type and to number of nodes per element
    template <class ElementType, DIM D, int N>
    class ElementSet {
        static constexpr int V = ElementType::nVertices;

    public:
        ElementSet(const Connectivity<N> & connectivity, const NodalField<real, D> & coordinates) :
            _connectivity(connectivity),
            _elements(connectivity, coordinates)
        {}

        virtual ~ElementSet() {}

        inline DIM dim() const { return D; }
        inline const Elements<ElementType, D> & elements() const { return _elements; }

    private:
        const Connectivity<N> & _connectivity;
        Elements<ElementType, D> _elements;
    };

    // template with respect to degree P
    template <size_t P>
    class ElementSetTri : public ElementSet<TRI, DIM2, (P + 1) * (P + 2) / 2> {
    public:
        template <DIM D>
        ElementSetTri(
            const Connectivity<(P + 1) * (P + 2) / 2> & connectivity,
            const NodalField<real, D> & coordinates) :
            ElementSet<TRI, D, (P + 1) * (P + 2) / 2>(connectivity, coordinates)
        {}

        ~ElementSetTri() {}
    };

}
#endif    //__MITO__ELEMENTS__

// end of file
