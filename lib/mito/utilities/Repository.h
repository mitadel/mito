// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// DESIGN NOTES

// A {Repository} is a data structure that is meant to store a collection of shareable resources of
// uniform type {T} in a central location. The {Repository} data structure is responsible for
// creating instances of resource {T} upon request of the client and erasing them when they are no
// longer used.
//
// Resources can only be 'checked out' from the repository wrapped into shared pointers, which keep
// track of the use of the resource through their lifetime. In fact, the {Repository} data structure
// is meant to closely collaborate with a {SharedPointer} class, which provides the information of
// resources being unused and therefore redundant. However, the {SharedPointer} class differs from
// the {shared_ptr} offered by the C++ standard library in two respects:
//      1) the {SharedPointer} does not have ownership of the resource and does not have privileges
//          to allocate and deallocate memory for it. In fact, the allocation and deallocation of
//          memory is entirely managed by the {Repository} class, which in turn delegates this to a
//          {SegmentedAllocator}, which allocates and deallocates resources collectively in chunks
//          to optimize memory allocation/deallocation. In this respect, the role of the
//          {SharedPointer} is only that of managing the book keeping on the count of references to
//          the resources.
//
//      2) the {SharedPointer} does not store the reference count directly, but manages a reference
//          count stored within the resource. This allows the {Repository} to synthesize a
//          {SharedPointer} on the fly, upon request of a given resource by a given client, with the
//          correct reference count.
//
// The only requirement of the {Repository} class on the template type {T} of its resources
// stored is simply that the resource is a {reference_countable_c}, which means that the
// resource is able to provide the machinery needed by the {SharedPointer} to do the book
// keeping. Specifically, this translates into the class of the resource inheriting from class
// {Shareable}, which provides the minimal interface necessary to collaborate with
// {SharedPointer} to perform reference counting. Ideally, the resource is otherwise immutable
// (i.e. except from the reference count attribute). The use of {Repository} for a class
// that is not immutable can be envisioned but has not been explored so far.
// Class {Shareable} also implements a method {is_valid} that assesses, based on the reference
// count, if the resource is being used or not. Unused resources are signed up to be overwritten
// by the next resource inserted in the container. Iterators to a segmented container are smart
// enough to skip the unused elements.

namespace mito::utilities {

    template <class sharedPointerT>
    class Repository {
        // requires reference_countable_c<sharedPointerT::resource_type>
      public:
        // me
        using repository_type = Repository<sharedPointerT>;
        // the shared resource
        using pointer_type = sharedPointerT;
        // my resource type
        using resource_type = typename sharedPointerT::resource_type;
        // typedef for a collection of resources
        using resource_collection_type = segmented_allocator_t<resource_type>;

        // iterators
        using iterator = SegmentedContainerIterator<repository_type>;

      public:
        // default constructor
        Repository(int segment_size) : _resources(segment_size) {}

        // destructor
        ~Repository()
        {
            // destroy all resources
            for (const auto & resource : _resources) {
                resource.~resource_type();
            }
        }

        // build a resource passing down {args...} to the resource constructor and store it in the
        // repository
        template <class... Args>
        auto emplace(Args &&... args) -> pointer_type
        {
            // get a spare location for the placement of the new resource
            auto [location, _] = _resources.location_for_placement();

            // create a new instance of {resource_type} at location {location} with placement new
            resource_type * resource = new (location) resource_type(std::forward<Args>(args)...);

            // add resource to the collection of resources
            _resources.insert(resource);

            // assign it to a new pointer
            pointer_type pointer(resource);

            // all done
            return pointer;
        }

        // erase a resource from the repository
        // (this method actually erases the simplex only if is no one else is using it, otherwise
        // does nothing)
        inline auto erase(pointer_type & resource) -> bool
        {
            // in the resource is {nullptr}, there is nothing to erase
            if (resource.is_nullptr()) {
                return false;
            }

            // remove this resource from the collection of resources
            _resources.erase(resource.handle());
            // destroy the resource
            resource->~resource_type();
            // reset the shared pointer to the resource
            resource.reset();

            // all done
            return true;
        }

        // returns the resource corresponding to this resource id
        static inline auto resource(index_t<resource_type> index) -> pointer_type
        {
            // fetch the resource based on the index
            auto resource = pointer_type::resource(index);
            // wrap the resource in a shared pointer
            return pointer_type(resource);
        }

        inline auto size() const -> int
        {
            // all done
            return std::size(_resources);
        }

        inline auto capacity() const -> int
        {
            // all done
            return _resources.capacity();
        }

      public:
        /**
         * iterators
         */
        constexpr auto begin() const -> iterator
        {
            // delegate answer to {_resources}
            return iterator(_resources.begin());
        }

        constexpr auto end() const -> iterator
        {
            // delegate answer to {_resources}
            return iterator(_resources.end());
        }

      private:
        // container to store the resources
        resource_collection_type _resources;
    };
}


// end of file
