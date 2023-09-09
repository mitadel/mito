// code guard
#if !defined(mito_utilities_Repository_h)
#define mito_utilities_Repository_h

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
//          {SegmentedContainer}, which allocates and deallocates resources collectively in chunks
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
// stored is simply that the resource is a {ReferenceCountedObject}, which means that the
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

    template <class sharedResourceT>
    class Repository {
        // requires ReferenceCountedObject<sharedResourceT::resource_type>
      public:
        // me
        using repository_type = Repository<sharedResourceT>;
        // the shared resource
        using shared_ptr_t = sharedResourceT;
        // my resource type
        using resource_t = typename sharedResourceT::resource_type;
        // typedef for a collection of resources
        using resource_collection_t = segmented_t<resource_t>;

        // iterators
        using iterator = RepositoryIterator<repository_type>;

      public:
        // default constructor
        Repository(int segment_size) : _resources(segment_size) {};

        // destructor
        ~Repository()
        {
            // destroy all resources
            for (const auto & resource : _resources) {
                resource.~resource_t();
            }
        }

        // build a resource passing down {args...} to the resource constructor and store it in the
        // repository
        template <class... Args>
        auto emplace(Args &&... args) -> shared_ptr_t
        {
            // get a spare location for the placement of the new resource
            auto location = _resources.location_for_placement();

            // create a new instance of {resource_t} at location {location} with placement new
            resource_t * resource = new (location) resource_t(args...);

            // assign it to a new pointer
            shared_ptr_t pointer(resource);

            // all done
            return pointer;
        }

        // erase a resource from the repository
        // (this method actually erases the simplex only if is no one else is using it, otherwise
        // does nothing)
        inline auto erase(shared_ptr_t & resource) -> void
        {
            // TOFIX: capture exception of invalid resource (nullptr)
            // sanity check
            assert(!resource.is_nullptr());
            // remove this resource from the collection of resources
            _resources.erase(resource.handle());
            // destroy the resource
            resource->~resource_t();
            // reset the shared pointer to the resource
            resource.reset();

            // all done
            return;
        }

        // returns the resource corresponding to this resource id
        static inline auto resource(index_t<resource_t> index) -> shared_ptr_t
        {
            // fetch the resource based on the index
            auto resource = shared_ptr_t::resource(index);
            // wrap the resource in a shared pointer
            return shared_ptr_t(resource);
        }

        inline auto resources() const -> const resource_collection_t &
        {
            // all done
            return _resources;
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
        resource_collection_t _resources;
    };
}

#endif    // mito_utilities_Repository_h

// end of file
