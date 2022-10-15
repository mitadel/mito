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

        // whether the resource is valid or not
        inline auto is_valid() const -> bool;

        // meta methods
      public:
        // destructor
        constexpr inline ~Shareable();

        // default constructor
        constexpr inline Shareable();

        // copy constructors
        inline Shareable(const Shareable &) = delete;
        inline Shareable(Shareable &) = delete;

        // move constructor
        inline Shareable(Shareable &&) = delete;

        // assignment operator
        inline Shareable & operator=(const Shareable &) = delete;
        inline Shareable & operator=(Shareable &) = delete;

        // move assignment operator
        inline Shareable & operator=(Shareable &&) = delete;

      private:
        // increment the reference count
        inline auto _acquire() -> int;
        // decrement the reference count
        inline auto _release() -> int;

        // data members
      private:
        int _reference_count;

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
