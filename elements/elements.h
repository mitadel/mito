#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"
#include "nodal_field.h"

namespace mito {

enum ElementType {TRI, TET};

class Connectivity {
  public:
    Connectivity(size_t nel, size_t nen) : _nel(nel), _nen(nen), _connectivityArray(nel * nen) {}
    Connectivity(size_t nel, size_t nen, std::vector<size_t> connectivityArray) : 
        _nel(nel), _nen(nen), _connectivityArray(connectivityArray) {}

    ~Connectivity() {}

    inline const size_t & operator()(size_t e, size_t a) const
    {
        return _connectivityArray[e * _nen + a ];
    }

    inline size_t & operator()(size_t e, size_t a)
    {
        return _connectivityArray[e * _nen + a ];
    }

    inline const size_t & operator()(size_t e) const
    {
        return _connectivityArray[e * _nen];
    }

    inline size_t & operator()(size_t e)
    {
        return _connectivityArray[e * _nen];
    }

    inline const std::vector<size_t> & operator()() const
    {
        return _connectivityArray;
    }

    inline std::vector<size_t> & operator()()
    {
        return _connectivityArray;
    }

  private:
    // number of elements
    size_t _nel;
    // number of nodes per element
    size_t _nen;
    // connectivity array
    std::vector<size_t> _connectivityArray; 
};

template<DIM D>
class ElementSet
{
  public:
    ElementSet(ElementType type, const Connectivity & connectivity, 
        const NodalField<real> & coordinates) : _type(type), _connectivity(connectivity) {}
    virtual ~ElementSet() {}

    ElementType type() const {return _type;}
    DIM dim() const {return D;};

  private: 
    ElementType _type;
    const Connectivity & _connectivity;
};

class ElementSetTri : public ElementSet<DIM2>
{
  public:
    ElementSetTri(const Connectivity & connectivity, const NodalField<real> & coordinates) : 
        ElementSet<DIM2>(TRI, connectivity, coordinates) {}
    ~ElementSetTri() {}

};

}
#endif //__MITO__ELEMENTS__

// end of file
