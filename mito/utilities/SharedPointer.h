// -*- C++ -*-
//
// michael a.g. aïvázis
// orthologue
// (c) 1998-2022 all rights reserved
//

// code guard
#if !defined(mito_utilities_SharedPointer_h)
#define mito_utilities_SharedPointer_h

// place SharedPointer in namespace mito::utilities
namespace mito { namespace utilities {
    template <class Resource, bool immortal>
    class SharedPointer;
}}


// declaration
template <class Resource, bool immortal = false>
class mito::utilities::SharedPointer {
    // types
  public:
    using resource_t = Resource;
    using handle_t = Resource::handle_t;
    using shared_t = Resource::shared_t;

    // interface
  public:
    inline operator handle_t() const;

    // meta methods
  public:
    inline ~SharedPointer();
    inline SharedPointer(handle_t);
    inline SharedPointer(const SharedPointer &);
    inline SharedPointer & operator=(const SharedPointer &);

    // data members
  private:
    shared_t * _shared;
};


// get the inline definitions
#define mito_utilities_SharedPointer_icc
#include "SharedPointer.icc"
#undef mito_utilities_SharedPointer_icc


#endif
// end of file
