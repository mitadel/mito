#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"
#include "nodal_field.h"

namespace mito {

enum ElementType {TRI, TET};

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
    Elements(size_t nelements) : _vertices(nelements) {}
    ~Elements() {}

  private:
    std::vector<mito::vector<D>> _vertices;
};

template<DIM D, size_t N>
class ElementSet
{
  public:
    ElementSet(ElementType type, const Connectivity<N> & connectivity, 
        const NodalField<real> & coordinates) : _type(type), _connectivity(connectivity), 
        _elements(connectivity.nelements()) {
            // TODO: Fill in _elements ...

            // all done
            return;
        }
    virtual ~ElementSet() {}

    inline ElementType type() const {return _type;}
    inline DIM dim() const {return D;}
    inline const Elements<D> & elements() const {return _elements;}

  private: 
    ElementType _type;
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
        : ElementSet<DIM2, (P + 1) * (P + 2) / 2>(TRI, connectivity, coordinates) {}
    ~ElementSetTri() {}

};

}
#endif //__MITO__ELEMENTS__

// end of file
