#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"
#include "nodal_field.h"

namespace mito {

/**
 *  Structs for the element type
 */

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
    Connectivity(int nel, std::vector<int> &&connectivityArray) : 
        _nel(nel), _connectivityArray(connectivityArray) {
            // assert that vector's dimension is compatible with nel and nen
            assert(_connectivityArray.size() == _nel * N);
            // all done
            return;
        }

    ~Connectivity() {}

    inline const int & operator()(int e, int a) const
    {
        assert(e < _nel && a < N);
        return _connectivityArray[e * N + a ];
    }

    inline int & operator()(int e, int a)
    {
        assert(e < _nel && a < N);
        return _connectivityArray[e * N + a ];
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

    inline const std::vector<int> & operator()() const
    {
        return _connectivityArray;
    }

    inline std::vector<int> & operator()()
    {
        return _connectivityArray;
    }

    inline int nElements() const
    {
        return _nel;
    }

  private:
    // number of elements
    int _nel;
    // connectivity array
    std::vector<int> _connectivityArray; 
};

// Collection of homogeneous elements
template <class ElementType, DIM D>
class Elements {

    static constexpr int V = ElementType::nVertices;

  private:

    template<int N>
    void _fillVertices(const Connectivity<N> & connectivity, const NodalField<real> & coordinates) {

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
    // fabs(det(a1, 1; ...; aV, 1)) / D!, with D = V - 1 being the dimension of the phyisical space
    void _computeJacobians() {
        
        static tensor<mito::DIM(D + 1)> verticesTensor;

        // TOFIX: This formula does not work for triangles in 3D, or for lines in 2D or 3D...
        static_assert(V == int(D)+1);

        for (auto e = 0; e < _nElements; ++e) {
            std::fill(verticesTensor.begin(), verticesTensor.end(), 0.0);
            for (auto a = 0; a < V; ++a) {
                for (auto d = 0; d < D; ++d) {
                    verticesTensor[a * V + d] = vertex(e, a)[d];
                }
                verticesTensor[a * V + D] = 1.0;
            }
            _jacobians[e] = fabs(ComputeDeterminant(verticesTensor)) / Factorial<D>();
        }

        return;
    }

  public:
    template<int N>
    Elements(const Connectivity<N> & connectivity, const NodalField<real> & coordinates) : 
        _nElements(connectivity.nElements()), _vertices(_nElements * V), _jacobians(_nElements) {

        // fill container with elements vertices based on connectivity and coordinates
        _fillVertices(connectivity, coordinates); 

        // compute the volume of each element 
        _computeJacobians();

        // all done
        return;
    }

    ~Elements() {}

    const mito::vector<D> & vertex(int e, int v) const {
        assert(e < _nElements && v < V);
        return _vertices[e * V + v];
    }

    mito::vector<D> & vertex(int e, int v) {
        assert(e < _nElements && v < V);
        return _vertices[e * V + v];
    }  

    inline int nElements() const {return _nElements;} 
    inline int nVertices() const {return V;} 
    inline real jacobian(int e) const {return _jacobians[e];} 

  private:
    int _nElements;
    std::vector<mito::vector<D> > _vertices;
    std::vector<real> _jacobians;
};

// template with respect to element type and to number of nodes per element
template<class ElementType, DIM D, int N>
class ElementSet
{
    static constexpr int V = ElementType::nVertices;

  public:

    ElementSet(const Connectivity<N> & connectivity, const NodalField<real> & coordinates) :
        _connectivity(connectivity), _elements(connectivity, coordinates) {}

    virtual ~ElementSet() {}

    inline DIM dim() const {return D;}
    inline const Elements<ElementType, D> & elements() const {return _elements;}

  private: 
    const Connectivity<N> & _connectivity;
    Elements<ElementType, D> _elements;
};

// template with respect to degree P
template <size_t P>
class ElementSetTri : public ElementSet<TRI, DIM2, (P + 1) * (P + 2) / 2>
{
  public:
    ElementSetTri(const Connectivity<(P + 1) * (P + 2) / 2> & connectivity, 
        const NodalField<real> & coordinates) 
        : ElementSet<TRI, DIM2, (P + 1) * (P + 2) / 2>(connectivity, coordinates) {}

    ~ElementSetTri() {}

};

}
#endif //__MITO__ELEMENTS__

// end of file
