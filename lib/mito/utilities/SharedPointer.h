// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SharedPointer_h)
#define mito_utilities_SharedPointer_h


namespace mito::utilities {

    // declaration
    template <class Resource>
    // requires ReferenceCountedObject<Resource>
    class SharedPointer {
        // types
      public:
        using shared_ptr_t = SharedPointer<Resource>;
        using resource_t = Resource;
        using handle_t = Resource *;

        // interface
      public:
        // returns the id of this (oriented) simplex
        inline auto id() const -> index_t<Resource>;

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
        inline SharedPointer(const SharedPointer<Resource> &);

        // move constructor
        inline SharedPointer(SharedPointer<Resource> &&) noexcept;

        // assignment operator
        inline SharedPointer & operator=(const SharedPointer<Resource> &);

        // move assignment operator
        inline SharedPointer & operator=(SharedPointer<Resource> &&);

      private:
        // accessor for {handle}
        inline auto handle() const noexcept -> handle_t;

        // returns the resource corresponding to this resource id
        static inline auto resource(index_t<Resource>) -> handle_t;

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

    template <class Resource>
    inline bool operator==(const SharedPointer<Resource> & lhs, const SharedPointer<Resource> & rhs)
    {
        return lhs.id() == rhs.id();
    }

    template <class Resource>
    inline bool operator<(const SharedPointer<Resource> & lhs, const SharedPointer<Resource> & rhs)
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
