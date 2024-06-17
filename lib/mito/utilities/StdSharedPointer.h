// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


namespace mito::utilities {

    // declaration
    template <class resourceT>
    class StdSharedPointer : public std::shared_ptr<resourceT> {
        // types
      public:
        using shared_ptr_type = StdSharedPointer<resourceT>;
        using resource_type = resourceT;

        // interface
      public:
        // returns the id of this (oriented) simplex
        inline auto id() const -> index_t<resource_type>;

        // meta methods
      public:
        // constructor
        template <class... Args>
        requires(std::is_constructible_v<resource_type, Args...>)
        inline StdSharedPointer(Args &&... args);

        // destructor
        inline ~StdSharedPointer() = default;

        // copy constructor
        inline StdSharedPointer(const shared_ptr_type &) = default;

        // move constructor
        inline StdSharedPointer(shared_ptr_type &&) noexcept = default;

        // assignment operator
        inline StdSharedPointer & operator=(const shared_ptr_type &) = default;

        // move assignment operator
        inline StdSharedPointer & operator=(shared_ptr_type &&) noexcept = default;
    };

    template <class resourceT>
    inline bool operator==(
        const StdSharedPointer<resourceT> & lhs, const StdSharedPointer<resourceT> & rhs)
    {
        return lhs.id() == rhs.id();
    }

    template <class resourceT>
    inline auto operator<=>(
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
