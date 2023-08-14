// code guard
#if !defined(mito_utilities_Repository_h)
#define mito_utilities_Repository_h

namespace mito::utilities {

    template <class resourceT>
    class Repository {
      public:
        // my resource type
        using resource_t = resourceT;
        // typedef for a collection of resources
        using resource_collection_t = segmented_t<shared_ptr<resource_t>>;

      public:
        // default constructor
        Repository(int segment_size) : _resources(segment_size) {};

        // destructor
        ~Repository()
        {
            // destroy all resources
            for (const auto & resource : _resources) {
                resource->~resource_t();
            }
        }

        // build a resource passing down {args...} to the resource constructor and store it in the
        // repository
        template <class... Args>
        auto emplace(Args &&... args) -> shared_ptr<resource_t>
        {
            // get a spare location for the placement of the new resource
            auto location = _resources._location_for_placement();

            // create a new instance of {resource_t} at location {location} with placement new
            resource_t * resource = new (location) resource_t(args...);

            // assign it to a new pointer
            shared_ptr<resource_t> pointer(resource);

            // all done
            return pointer;
        }

        // erase a resource from the repository
        // (this method actually erases the simplex only if is no one else is using it, otherwise
        // does nothing)
        inline auto erase(shared_ptr<resource_t> & resource) -> void
        {
            // // if the repository is the last user of the resource
            // if (resource.references() == 1) {
            // remove this resource from the collection of resources
            _resources._erase(resource);
            // destroy the resource
            resource->~resource_t();
            // QUESTION: to reset or not to reset?
            // reset the shared pointer to the resource
            resource.reset();
            // }

            // all done
            return;
        }

        inline auto resources() const -> const resource_collection_t &
        {
            // all done
            return _resources;
        }

      private:
        // container to store the resources
        resource_collection_t _resources;
    };
}

#endif    // mito_utilities_Repository_h

// end of file
