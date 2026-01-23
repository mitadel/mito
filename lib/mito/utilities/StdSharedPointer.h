// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


namespace mito::utilities {

    // declaration
    template <class resourceT>
    class StdSharedPointer {
        // types
      public:
        using shared_ptr_type = StdSharedPointer<resourceT>;
        using resource_type = resourceT;
        using handle_type = resourceT *;

        // interface
      public:
        // returns the id of this (oriented) simplex
        constexpr auto id() const -> index_t<resource_type>;

        // operator->
        constexpr auto operator->() const noexcept -> handle_type;

        // meta methods
      public:
        // constructor
        template <class... Args>
        requires(std::is_constructible_v<resource_type, Args...>)
        constexpr StdSharedPointer(Args &&... args);

        // destructor
        constexpr ~StdSharedPointer() = default;

        // copy constructor
        constexpr StdSharedPointer(const shared_ptr_type &) = default;

        // move constructor
        constexpr StdSharedPointer(shared_ptr_type &&) noexcept = default;

        // assignment operator
        constexpr StdSharedPointer & operator=(const shared_ptr_type &) = default;

        // move assignment operator
        constexpr StdSharedPointer & operator=(shared_ptr_type &&) noexcept = default;

        // data members
      private:
        std::shared_ptr<resourceT> _ptr;
    };

    template <class resourceT>
    constexpr bool operator==(
        const StdSharedPointer<resourceT> & lhs, const StdSharedPointer<resourceT> & rhs)
    {
        return lhs.id() == rhs.id();
    }

    template <class resourceT>
    constexpr auto operator<=>(
        const StdSharedPointer<resourceT> & lhs, const StdSharedPointer<resourceT> & rhs)
    {
        // delegate to ids
        return lhs.id() <=> rhs.id();
    }
}


// get the inline definitions
#define mito_utilities_StdSharedPointer_icc
#include "StdSharedPointer.icc"
#undef mito_utilities_StdSharedPointer_icc


// end of file
