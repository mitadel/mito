#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"
#include "nodal_field.h"

namespace mito {

// TOFIX: Instead of this:

enum ElementType {TRI, TET};

template <ElementType type>
inline int nVertices();
        
template <>
inline int nVertices<TRI>(){
    return 3;
}

template <>
inline int nVertices<TET>(){
    return 4;
}

template <ElementType type>
constexpr inline DIM parametricDim();
        
template <>
constexpr inline DIM parametricDim<TRI>(){
    return DIM3;
}

template <>
constexpr inline DIM parametricDim<TET>(){
    return DIM4;
}

template <ElementType type>
constexpr inline DIM physicalDim();
        
template <>
constexpr inline DIM physicalDim<TRI>(){
    return DIM2;
}

template <>
constexpr inline DIM physicalDim<TET>(){
    return DIM3;
}

template <ElementType type>
inline std::string name();
        
template <>
inline std::string name<TRI>(){
    return "TRI";
}

template <>
inline std::string name<TET>(){
    return "TET";
}

// TOFIX:
//        consider this:
/*
struct TRI {
  static constexpr std::string name = "TRI";
  static constexpr int physicalDim = 2;
  static constexpr int parametricDim = physicalDim + 1;
  static constexpr int nVertices = physicalDim + 1;
}

struct TET {
  static constexpr std::string name = "TET";
  static constexpr int physicalDim = 3;
  static constexpr int parametricDim = physicalDim + 1;
  static constexpr int nVertices = physicalDim + 1;
}
*/
//        The advantage is to collect all in the same place the info that element types will need 
//        to provide. Also, since we are using templates, we do not need TRI and TET to be of the 
//        same type but just to be able to provide this information.  

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

template <DIM D>
class Elements {
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

// template with respect to physical dimension D, and to number of nodes per element
template<DIM D, int N>
class ElementSet
{
  public:
    ElementSet(ElementType type, int nVertices, const Connectivity<N> & connectivity, 
        const NodalField<real> & coordinates) : _type(type), _nVertices(nVertices), 
        _connectivity(connectivity), _elements(connectivity.nElements(), _nVertices) {
            // TODO: Fill in _elements ...

            for (auto e = 0; e < _connectivity.nElements(); ++e) {
                for (auto a = 0; a < _nVertices; ++a) {
                    // TOFIX: Here we assume that the vertices are the first _nVertices entries of 
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

    inline ElementType type() const {return _type;}
    inline DIM dim() const {return D;}
    inline const Elements<D> & elements() const {return _elements;}

  private: 
    ElementType _type;
    // number of vertices per element
    int _nVertices;
    const Connectivity<N> & _connectivity;
    Elements<D> _elements;
};

// template with respect to degree P
template <int P>
class ElementSetTri : public ElementSet<DIM2, (P + 1) * (P + 2) / 2>
{
  public:
    ElementSetTri(const Connectivity<(P + 1) * (P + 2) / 2> & connectivity, 
        const NodalField<real> & coordinates) 
        : ElementSet<DIM2, (P + 1) * (P + 2) / 2>(TRI, nVertices<TRI>(), connectivity, 
            coordinates) {}
    ~ElementSetTri() {}

};

}
#endif //__MITO__ELEMENTS__

// end of file
