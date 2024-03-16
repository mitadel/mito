// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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
        constexpr auto is_valid() const noexcept -> bool;

        // invalidate the resource
        constexpr auto invalidate() noexcept -> void;

        // meta methods
      public:
        // destructor
        constexpr virtual ~Invalidatable();

        // default constructor
        constexpr Invalidatable();

        // copy constructors
        constexpr Invalidatable(const Invalidatable &) = default;
        constexpr Invalidatable(Invalidatable &) = default;

        // move constructor
        constexpr Invalidatable(Invalidatable &&) noexcept = default;

        // assignment operator
        constexpr Invalidatable & operator=(const Invalidatable &) = default;
        constexpr Invalidatable & operator=(Invalidatable &) = default;

        // move assignment operator
        constexpr Invalidatable & operator=(Invalidatable &&) noexcept = default;

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
