// code guard
#if !defined(mito_utilities_api_h)
#define mito_utilities_api_h


namespace mito::utilities {

    // shared pointer alias
    template <class resourceT>
    using shared_ptr = SharedPointer<resourceT>;

    // TOFIX: perhaps eventually remove indices?
    // index type alias
    template <class resourceT>
    using index_t = std::uintptr_t;

    // segmented container alias
    template <class sharedResourceT>
    using segmented_t = SegmentedContainer<sharedResourceT>;

    // the repository
    template <class sharedResourceT>
    using repository_t = class Repository<sharedResourceT>;
}


#endif

// end of file
