// -*- C++ -*-
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
        using shared_ptr_t = SharedPointer<resourceT>;
        using resource_t = resourceT;
        using handle_t = resourceT *;

        // interface
      public:
        // returns the id of this (oriented) simplex
        inline auto id() const -> index_t<resource_t>;

        // accessor for the number of outstanding references
        inline auto references() const -> int;

        // check if the handle is the null pointer
        inline auto is_nullptr() const noexcept -> bool;

        // operator->
        auto operator->() const noexcept -> handle_t;

        // // operator*
        // auto operator*() const -> const resource_t &;

        // meta methods
      public:
        // destructor
        inline ~SharedPointer();

        // default constructor
        inline SharedPointer();

        // constructor
        inline SharedPointer(handle_t);

        // copy constructor
        inline SharedPointer(const shared_ptr_t &);

        // move constructor
        inline SharedPointer(shared_ptr_t &&) noexcept;

        // assignment operator
        inline shared_ptr_t & operator=(const shared_ptr_t &);

        // move assignment operator
        inline shared_ptr_t & operator=(shared_ptr_t &&);

      private:
        // accessor for {handle}
        inline auto handle() const noexcept -> handle_t;

        // returns the resource corresponding to this resource id
        static inline auto resource(index_t<resource_t>) -> handle_t;

        // reset the shared pointer
        inline auto reset() -> void;

        // increment the reference count
        inline auto _acquire() const -> void;
        // decrement the reference count
        inline auto _release() const -> void;

        // data members
      private:
        // handle to the resource
        handle_t _handle;

      private:
        // friendship with Repository
        friend class Repository<shared_ptr_t>;
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
