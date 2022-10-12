// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SharedPointer_h)
#define mito_utilities_SharedPointer_h


namespace mito::utilities {

    // declaration
    template <class Resource>
    requires ReferenceCountedObject<Resource>
    class SharedPointer {
        // types
      public:
        using resource_t = Resource;
        using handle_t = Resource *;

        // interface
      public:
        // cast to handle_t
        inline operator handle_t() const;

        // accessor for the number of outstanding references
        inline auto references() const -> int;

        // operator->
        auto operator->() const -> handle_t;

        // meta methods
      public:
        // destructor
        inline ~SharedPointer();

        // default constructor
        inline SharedPointer();

        // constructor
        inline SharedPointer(handle_t);

        // copy constructor
        inline SharedPointer(const SharedPointer<Resource> &);

        // move constructor
        inline SharedPointer(SharedPointer<Resource> &&);

        // assignment operator
        inline SharedPointer & operator=(const SharedPointer<Resource> &);

        // move assignment operator
        inline SharedPointer & operator=(SharedPointer<Resource> &&);

      private:
        // increment the reference count
        inline auto _acquire() -> int;
        // decrement the reference count
        inline auto _release() -> int;

        // data members
      private:
        handle_t _handle;
    };
}


// get the inline definitions
#define mito_utilities_SharedPointer_icc
#include "SharedPointer.icc"
#undef mito_utilities_SharedPointer_icc


#endif
// end of file
