// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_Invalidatable_h)
#define mito_utilities_Invalidatable_h

namespace mito::utilities {

    // declaration
    class Invalidatable {

        // interface
      public:
        // whether the resource is valid or not
        inline auto is_valid() const noexcept -> bool;

        // invalidate the resource
        inline auto invalidate() noexcept -> void;

        // meta methods
      public:
        // destructor
        constexpr inline virtual ~Invalidatable();

        // default constructor
        constexpr inline Invalidatable();

        // copy constructors
        inline Invalidatable(const Invalidatable &) = delete;
        inline Invalidatable(Invalidatable &) = delete;

        // move constructor
        inline Invalidatable(Invalidatable &&) = delete;

        // assignment operator
        inline Invalidatable & operator=(const Invalidatable &) = delete;
        inline Invalidatable & operator=(Invalidatable &) = delete;

        // move assignment operator
        inline Invalidatable & operator=(Invalidatable &&) = delete;

        // data members
      private:
        bool _is_valid;
    };
}


// get the inline definitions
#define mito_utilities_Invalidatable_icc
#include "Invalidatable.icc"
#undef mito_utilities_Invalidatable_icc


#endif
// end of file
