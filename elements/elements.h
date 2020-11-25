#ifndef __MITO__ELEMENTS__
#define __MITO__ELEMENTS__

#include "../mito.h"

namespace mito {

enum ElementType {TRI, TET};

template<DIM D>
class ElementSet
{
  public:

    ElementSet(ElementType type) : _type(type) {}
    virtual ~ElementSet() {}

    ElementType type() const {return _type;}
    DIM dim() const {return D;};

  private: 
    ElementType _type;
};

class ElementSetTri : public ElementSet<DIM2>
{
  public:
    ElementSetTri() : ElementSet<DIM2>(TRI) {}
    ~ElementSetTri() {}

};

}
#endif //__MITO__ELEMENTS__

// end of file
