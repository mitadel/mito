// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#if !defined(mito_utilities_SharedPointer_h)
#define mito_utilities_SharedPointer_h


namespace mito::utilities {

    // declaration
    template <class resourceT>
    // requires ReferenceCountedObject<resourceT>
    class SharedPointer {
        // types
      public:
        using shared_ptr_type = SharedPointer<resourceT>;
        using resource_type = resourceT;
        using handle_type = resourceT *;

        // interface
      public:
        // returns the id of this (oriented) simplex
        inline auto id() const -> index_t<resource_type>;

        // accessor for the number of outstanding references
        inline auto references() const -> int;

        // check if the handle is the null pointer
        inline auto is_nullptr() const noexcept -> bool;

        // operator->
        auto operator->() const noexcept -> handle_type;

        // // operator*
        // auto operator*() const -> const resource_type &;

        // meta methods
      public:
        // destructor
        inline ~SharedPointer();

        // default constructor
        inline SharedPointer();

        // constructor
        inline SharedPointer(handle_type);

        // copy constructor
        inline SharedPointer(const shared_ptr_type &);

        // move constructor
        inline SharedPointer(shared_ptr_type &&) noexcept;

        // assignment operator
        inline shared_ptr_type & operator=(const shared_ptr_type &);

        // move assignment operator
        inline shared_ptr_type & operator=(shared_ptr_type &&) noexcept;

      private:
        // accessor for {handle}
        inline auto handle() const noexcept -> handle_type;

        // returns the resource corresponding to this resource id
        static inline auto resource(index_t<resource_type>) -> handle_type;

        // reset the shared pointer
        inline auto reset() -> void;

        // increment the reference count
        inline auto _acquire() const -> void;
        // decrement the reference count
        inline auto _release() const -> void;

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
    inline bool operator<(
        const SharedPointer<resourceT> & lhs, const SharedPointer<resourceT> & rhs)
    {
        return lhs.id() < rhs.id();
    }
}


// get the inline definitions
#define mito_utilities_SharedPointer_icc
#include "SharedPointer.icc"
#undef mito_utilities_SharedPointer_icc


#endif
// end of file
