// -*- C++ -*-
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
    using handle_t = Resource *;

    // interface
  public:
    // cast to handle_t
    inline operator handle_t() const;
    // accessor for the number of outstanding references
    inline int references() const;

    // meta methods
  public:
    // destructor
    inline ~SharedPointer();
    // constructor
    inline SharedPointer(handle_t);
    // copy constructor
    inline SharedPointer(const SharedPointer &);
    // operator=
    inline SharedPointer & operator=(const SharedPointer &);

  private:
    // increment the reference count
    inline int _acquire();
    // decrement the reference count
    inline int _release();

    // data members
  private:
    int _count;
    handle_t _handle;
};


// get the inline definitions
#define mito_utilities_SharedPointer_icc
#include "SharedPointer.icc"
#undef mito_utilities_SharedPointer_icc


#endif
// end of file
