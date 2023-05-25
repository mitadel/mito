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
        // accessor for {handle}
        inline auto handle() const -> handle_t;

        // accessor for the number of outstanding references
        inline auto references() const -> int;

        // reset the shared pointer
        inline auto reset() -> void;

        // operator->
        auto operator->() const -> handle_t;

        // // operator*
        // auto operator*() const -> const resource_t &;

        // meta methods
      public:
        // destructor
        inline ~SharedPointer();

        // default constructor
        inline SharedPointer();

        // constructor
        inline SharedPointer(handle_t, segmented_t<shared_ptr_t> *);

        // copy constructor
        inline SharedPointer(const SharedPointer<Resource> &);

        // move constructor
        inline SharedPointer(SharedPointer<Resource> &&);

        // assignment operator
        inline SharedPointer & operator=(const SharedPointer<Resource> &);

        // move assignment operator
        inline SharedPointer & operator=(SharedPointer<Resource> &&);

      private:
        // increment the reference count
        inline auto _acquire() const -> void;
        // decrement the reference count
        inline auto _release() const -> void;

        // data members
      private:
        // handle to the resource
        handle_t _handle;
        // reference to the segmented container, which owns the memory
        segmented_t<shared_ptr_t> * _container;

      private:
        // friendship with SegmentedContainer
        friend class utilities::SegmentedContainer<resource_t>;
    };

    template <class Resource>
    inline bool operator==(const SharedPointer<Resource> & lhs, const SharedPointer<Resource> & rhs)
    {
        return lhs.handle() == rhs.handle();
    }

    template <class Resource>
    inline bool operator<(const SharedPointer<Resource> & lhs, const SharedPointer<Resource> & rhs)
    {
        return lhs.handle() < rhs.handle();
    }
}


// get the inline definitions
#define mito_utilities_SharedPointer_icc
#include "SharedPointer.icc"
#undef mito_utilities_SharedPointer_icc


#endif
// end of file
