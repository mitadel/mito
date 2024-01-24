// code guard
#if !defined(mito_utilities_SegmentedVector_h)
#define mito_utilities_SegmentedVector_h

// DESIGN NOTES

// Class {SegmentedVector} implements a data structure that is resizable, while also being stable
// in memory and able to leverage the efficiency of concurrent contiguous memory allocation in
// large chunks, as opposed to fragmented allocation.
//
// A {SegmentedVector} is articulated in multiple segments of memory, each internally contiguous.
// See the {SegmentedAllocator} class for more information on memory management.
//
// Resources can be erased from the {SegmentedVector}. Erasure consists of the resource being marked
// as invalid and then erased from the {SegmentedAllocator}.

namespace mito::utilities {

    template <class resourceT>
    requires InvalidatableObject<resourceT>
    class SegmentedVector {
      public:
        // me
        using segmented_vector_type = SegmentedVector<resourceT>;
        // my resource type
        using resource_type = resourceT;
        // my resource pointer type
        using pointer_type = resource_type *;
        // typedef for a collection of resources
        using resource_collection_type = segmented_allocator_t<resource_type>;

        // iterators
        using iterator = SegmentedContainerIterator<segmented_vector_type>;

      public:
        // default constructor
        SegmentedVector(int segment_size) : _resources(segment_size) {};

        // destructor
        ~SegmentedVector()
        {
            // destroy all resources
            for (const auto & resource : _resources) {
                resource.~resource_type();
            }
        }

        // build a resource passing down {args...} to the resource constructor
        template <class... Args>
        auto emplace_back(Args &&... args) -> iterator
        {
            // get a spare location for the placement of the new resource
            auto location = _resources.location_for_placement();

            // create a new instance of {resource_type} at location {location} with placement new
            resource_type * resource = new (location) resource_type(args...);

            // add resource to the collection of resources
            return iterator(_resources.insert(resource));
        }

        // erase a resource from the vector
        inline auto erase(iterator & resource) -> void
        {

            // sanity check
            assert((*resource)->is_valid());

            // invalidate the resource
            (*resource)->invalidate();
            // remove this resource from the collection of resources
            _resources.erase(*resource);

            // all done
            return;
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

#endif    // mito_utilities_SegmentedVector_h

// end of file
