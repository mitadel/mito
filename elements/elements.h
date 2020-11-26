#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"
#include "nodal_field.h"

namespace mito {

enum ElementType {TRI, TET};

template <ElementType type>
inline size_t nvertices();
        
template <>
inline size_t nvertices<TRI>(){
    return 3;
}

template <>
inline size_t nvertices<TET>(){
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

// template with respect to N, number of nodes per element
template <size_t N>
class Connectivity {
  public:
    Connectivity(size_t nel) : _nel(nel), _connectivityArray(nel * N, 0) {}
    Connectivity(size_t nel, std::vector<size_t> &&connectivityArray) : 
        _nel(nel), _connectivityArray(connectivityArray) {
            // assert that vector's dimension is compatible with nel and nen
            assert(_connectivityArray.size() == _nel * N);
            // all done
            return;
        }

    ~Connectivity() {}

    inline const size_t & operator()(size_t e, size_t a) const
    {
        assert(e < _nel && a < N);
        return _connectivityArray[e * N + a ];
    }

    inline size_t & operator()(size_t e, size_t a)
    {
        assert(e < _nel && a < N);
        return _connectivityArray[e * N + a ];
    }

    inline const size_t & operator()(size_t e) const
    {
        assert(e < _nel);
        return _connectivityArray[e * N];
    }

    inline size_t & operator()(size_t e)
    {
        assert(e < _nel);
        return _connectivityArray[e * N];
    }

    inline const std::vector<size_t> & operator()() const
    {
        return _connectivityArray;
    }

    inline std::vector<size_t> & operator()()
    {
        return _connectivityArray;
    }

    inline size_t nelements() const
    {
        return _nel;
    }

  private:
    // number of elements
    size_t _nel;
    // connectivity array
    std::vector<size_t> _connectivityArray; 
};

template <DIM D>
class Elements {
  public:
    Elements(size_t nelements, size_t nvertices) : _nelements(nelements), 
        _nvertices(nvertices), _vertices(nelements * nvertices) {}
    ~Elements() {}

    const mito::vector<D> & vertex(size_t e, size_t v) const {
        assert(e < _nelements && v < _nvertices);
        return _vertices[e * _nvertices + v];
    }

    mito::vector<D> & vertex(size_t e, size_t v) {
        assert(e < _nelements && v < _nvertices);
        return _vertices[e * _nvertices + v];
    }  

    size_t nelements() const {return _nelements;} 
    size_t nvertices() const {return _nvertices;} 

  private:
    size_t _nelements;
    size_t _nvertices;
    std::vector<mito::vector<D> > _vertices;
};

// template with respect to physical dimension D, and to number of nodes per element
template<DIM D, size_t N>
class ElementSet
{
  public:
    ElementSet(ElementType type, size_t nvertices, const Connectivity<N> & connectivity, 
        const NodalField<real> & coordinates) : _type(type), _nvertices(nvertices), 
        _connectivity(connectivity), _elements(connectivity.nelements(), _nvertices) {
            // TODO: Fill in _elements ...

            for (size_t e = 0; e < _connectivity.nelements(); ++e) {
                for (size_t a = 0; a < _nvertices; ++a) {
                    // TOFIX: Here we assume that the vertices are the first _nvertices entries of 
                    //       the connectivity...
                    const size_t & id = connectivity(e, a);
                    for (size_t d = 0; d < D; ++d) {
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
    size_t _nvertices;
    const Connectivity<N> & _connectivity;
    Elements<D> _elements;
};

// template with respect to degree P
template <size_t P>
class ElementSetTri : public ElementSet<DIM2, (P + 1) * (P + 2) / 2>
{
  public:
    ElementSetTri(const Connectivity<(P + 1) * (P + 2) / 2> & connectivity, 
        const NodalField<real> & coordinates) 
        : ElementSet<DIM2, (P + 1) * (P + 2) / 2>(TRI, nvertices<TRI>(), connectivity, 
            coordinates) {}
    ~ElementSetTri() {}

};

}
#endif //__MITO__ELEMENTS__

// end of file
