// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


namespace mito::utilities {

    // declaration
    template <class resourceT>
    // requires reference_countable_c<resourceT>
    class SharedPointer {
        // types
      public:
        using shared_ptr_type = SharedPointer<resourceT>;
        using resource_type = resourceT;
        using handle_type = resourceT *;

        // interface
      public:
        // returns the id of this (oriented) simplex
        constexpr auto id() const -> index_t<resource_type>;

        // accessor for the number of outstanding references
        constexpr auto references() const -> int;

        // check if the handle is the null pointer
        constexpr auto is_nullptr() const noexcept -> bool;

        // operator->
        constexpr auto operator->() const noexcept -> handle_type;

        // // operator*
        // auto operator*() const -> const resource_type &;

        // meta methods
      public:
        // destructor
        constexpr ~SharedPointer();

        // default constructor
        constexpr SharedPointer();

        // constructor
        constexpr SharedPointer(handle_type);

        // copy constructor
        constexpr SharedPointer(const shared_ptr_type &);

        // move constructor
        constexpr SharedPointer(shared_ptr_type &&) noexcept;

        // assignment operator
        constexpr shared_ptr_type & operator=(const shared_ptr_type &);

        // move assignment operator
        constexpr shared_ptr_type & operator=(shared_ptr_type &&) noexcept;

      private:
        // accessor for {handle}
        constexpr auto handle() const noexcept -> handle_type;

        // returns the resource corresponding to this resource id
        static constexpr auto resource(index_t<resource_type>) -> handle_type;

        // reset the shared pointer
        constexpr auto reset() -> void;

        // increment the reference count
        constexpr auto _acquire() const -> void;
        // decrement the reference count
        constexpr auto _release() const -> void;

        // data members
      private:
        // handle to the resource
        handle_type _handle;

      private:
        // friendship with Repository
        friend class Repository<shared_ptr_type>;
    };

    template <class resourceT>
    inline bool operator==(
        const SharedPointer<resourceT> & lhs, const SharedPointer<resourceT> & rhs)
    {
        return lhs.id() == rhs.id();
    }

    template <class resourceT>
    inline auto operator<=>(
        const SharedPointer<resourceT> & lhs, const SharedPointer<resourceT> & rhs)
    {
        // delegate to ids
        return lhs.id() <=> rhs.id();
    }
}


// get the inline definitions
#define mito_utilities_SharedPointer_icc
#include "SharedPointer.icc"
#undef mito_utilities_SharedPointer_icc


// end of file
