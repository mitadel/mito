// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_Shareable_h)
#define mito_utilities_Shareable_h

namespace mito::utilities {

    // declaration
    class Shareable {

        // interface
      public:
        // accessor for the number of outstanding references
        inline auto references() const -> int;

        // meta methods
      public:
        // destructor
        inline ~Shareable();

        // default constructor
        inline Shareable();

        // copy constructors
        inline Shareable(const Shareable &) = delete;
        inline Shareable(Shareable &);

        // move constructor
        inline Shareable(Shareable &&);

        // assignment operator
        inline Shareable & operator=(const Shareable &) = delete;
        inline Shareable & operator=(Shareable &);

        // move assignment operator
        inline Shareable & operator=(Shareable &&);

      private:
        // increment the reference count
        inline auto _acquire() -> int;
        // decrement the reference count
        inline auto _release() -> int;

        // data members
      private:
        int * _reference_count;

      private:
        // friendship with SharedPointer (the shared pointer needs r/w access to the reference count
        // of the Shareable instance)
        template <class T>
        requires mito::utilities::ReferenceCountedObject<T>
        friend class mito::utilities::SharedPointer;
    };
}


// get the inline definitions
#define mito_utilities_Shareable_icc
#include "Shareable.icc"
#undef mito_utilities_Shareable_icc


#endif
// end of file
