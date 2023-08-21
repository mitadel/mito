// code guard
#if !defined(mito_utilities_Repository_h)
#define mito_utilities_Repository_h

namespace mito::utilities {

    template <class sharedResourceT>
    class Repository {
        // requires ReferenceCountedObject<sharedResourceT::resource_type>
      public:
        using shared_ptr_t = sharedResourceT;
        // my resource type
        using resource_t = typename sharedResourceT::resource_type;
        // typedef for a collection of resources
        using resource_collection_t = segmented_t<sharedResourceT>;

      public:
        // default constructor
        Repository(int segment_size) : _resources(segment_size) {};

        // destructor
        ~Repository()
        {
            if (std::size(_resources) > 0) {
                // destroy all resources
                for (const auto & resource : _resources) {
                    resource->~resource_t();
                }
            }
        }

        // build a resource passing down {args...} to the resource constructor and store it in the
        // repository
        template <class... Args>
        auto emplace(Args &&... args) -> shared_ptr_t
        {
            // get a spare location for the placement of the new resource
            auto location = _resources._location_for_placement();

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
            // if the resource is valid
            if (!resource.is_nullptr()) {
                // remove this resource from the collection of resources
                _resources.erase(resource.handle());
                // destroy the resource
                resource->~resource_t();
                // QUESTION: to reset or not to reset?
                // reset the shared pointer to the resource
                resource.reset();
            }

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
        constexpr auto begin() const
        {
            // delegate answer to {_resources}
            return _resources.begin();
        }

        constexpr auto end() const
        {
            // delegate answer to {_resources}
            return _resources.end();
        }

      private:
        // container to store the resources
        resource_collection_t _resources;
    };
}

#endif    // mito_utilities_Repository_h

// end of file
