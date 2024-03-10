// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#if !defined(mito_utilities_Shareable_h)
#define mito_utilities_Shareable_h

namespace mito::utilities {

    // declaration
    class Shareable {

        // interface
      public:
        // whether the resource is valid or not
        inline auto is_valid() const noexcept -> bool;

        // meta methods
      public:
        // destructor
        constexpr inline virtual ~Shareable();

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
        // accessor for the number of outstanding references
        inline auto _references() const noexcept -> int;
        // increment the reference count
        inline auto _acquire() const noexcept -> int;
        // decrement the reference count
        inline auto _release() const noexcept -> int;

        // data members
      private:
        mutable int _reference_count;

      private:
        // friendship with SharedPointer (the shared pointer needs r/w access to the reference count
        // of the Shareable instance)
        template <class T>
        // requires ReferenceCountedObject<T>
        friend class utilities::SharedPointer;
    };
}


// get the inline definitions
#define mito_utilities_Shareable_icc
#include "Shareable.icc"
#undef mito_utilities_Shareable_icc


#endif
// end of file
