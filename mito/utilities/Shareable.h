// -*- C++ -*-
//
// michael a.g. aïvázis
// orthologue
// (c) 1998-2022 all rights reserved
//

// code guard
#if !defined(mito_utilities_Shareable_h)
#define mito_utilities_Shareable_h

// place Shareable in namespace mito::utilities
namespace mito { namespace utilities {
    template <class Resource, bool immortal>
    class Shareable;
}}


// declaration
template <class Resource, bool immortal>
class mito::utilities::Shareable {
    // types
  public:
    using resource_t = Resource;
    using handle_t = Resource::handle_t;

    // interface
  public:
    inline int acquire();             // increment the reference count
    inline int release();             // decrement the reference count
    inline int references() const;    // return the number of outstanding references

    inline handle_t handle() const;    // return the low level handle

    // meta methods
  public:
    inline ~Shareable();
    inline Shareable(handle_t);

    // disallow the copy constructors
  private:
    inline Shareable(const Shareable &);
    inline const Shareable & operator=(const Shareable &);

    // data members
  private:
    int _count;
    handle_t _handle;
};


// get the inline definitions
#define mito_utilities_Shareable_icc
#include "Shareable.icc"
#undef mito_utilities_Shareable_icc


#endif
// end of file
