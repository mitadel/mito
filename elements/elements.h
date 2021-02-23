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
  static constexpr DIM physicalDim = DIM2;
  static constexpr DIM parametricDim = DIM3;
  static constexpr int nVertices = physicalDim + 1;
};

struct TET {
  static constexpr auto name = "TET";
  static constexpr DIM physicalDim = DIM3;
  static constexpr DIM parametricDim = DIM4;
  static constexpr int nVertices = physicalDim + 1;
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
template <class ElementType>
class Elements {

    static const DIM D = ElementType::physicalDim;

  public:
    Elements(int nElements, int nVertices) : _nElements(nElements), 
        _nVertices(nVertices), _vertices(nElements * nVertices), _jacobians(nElements) {
            // compute jacobians

            // all done
            return;
        }
    ~Elements() {}

    const mito::vector<D> & vertex(int e, int v) const {
        assert(e < _nElements && v < _nVertices);
        return _vertices[e * _nVertices + v];
    }

    mito::vector<D> & vertex(int e, int v) {
        assert(e < _nElements && v < _nVertices);
        return _vertices[e * _nVertices + v];
    }  

    inline int nElements() const {return _nElements;} 
    inline int nVertices() const {return _nVertices;} 
    inline real jacobian(int e) const {return _jacobians[e];} 

  private:
    int _nElements;
    int _nVertices;
    std::vector<mito::vector<D> > _vertices;
    std::vector<real> _jacobians;
};

// template with respect to element type and to number of nodes per element
template<class ElementType, size_t N>
class ElementSet
{
    static const DIM D = ElementType::physicalDim;
    static const size_t nVertices = ElementType::nVertices;

  public:

    ElementSet(const Connectivity<N> & connectivity, const NodalField<real> & coordinates) :
        _connectivity(connectivity), _elements(connectivity.nElements(), nVertices) {
            // TODO: Fill in _elements ...

            for (auto e = 0; e < _connectivity.nElements(); ++e) {
                for (auto a = 0; a < nVertices; ++a) {
                    // TOFIX: Here we assume that the vertices are the first nVertices entries of 
                    //       the connectivity...
                    const int & id = connectivity(e, a);
                    for (auto d = 0; d < D; ++d) {
                        _elements.vertex(e, a)[d] = coordinates(id, d);
                    }
                }
            }

            // all done
            return;
        }

    virtual ~ElementSet() {}

    inline DIM dim() const {return D;}
    inline const Elements<ElementType> & elements() const {return _elements;}

  private: 
    const Connectivity<N> & _connectivity;
    Elements<ElementType> _elements;
};

// template with respect to degree P
template <size_t P>
class ElementSetTri : public ElementSet<TRI, (P + 1) * (P + 2) / 2>
{
  public:
    ElementSetTri(const Connectivity<(P + 1) * (P + 2) / 2> & connectivity, 
        const NodalField<real> & coordinates) 
        : ElementSet<TRI, (P + 1) * (P + 2) / 2>(connectivity, coordinates) {}

    ~ElementSetTri() {}

};

}
#endif //__MITO__ELEMENTS__

// end of file
