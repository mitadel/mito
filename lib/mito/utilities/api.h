// code guard
#if !defined(mito_utilities_api_h)
#define mito_utilities_api_h


namespace mito::utilities {

    // TOFIX: uniform notation Resource, ResourceT, resourceT, ...
    // shared pointer alias
    template <class Resource>
    using shared_ptr = SharedPointer<Resource>;

    // TOFIX: perhaps eventually remove indices?
    // index type alias
    template <class Resource>
    using index_t = std::uintptr_t;

    // segmented container alias
    template <class T>
    using segmented_t = SegmentedContainer<typename T::resource_t>;

    // the repository
    template <class resourceT>
    using repository_t = class Repository<resourceT>;

}


#endif

// end of file
