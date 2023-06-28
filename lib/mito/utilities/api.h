// code guard
#if !defined(mito_utilities_api_h)
#define mito_utilities_api_h


namespace mito::utilities {

    // shared pointer alias
    template <class Resource>
    using shared_ptr = SharedPointer<Resource>;

    // index type alias
    template <class Resource>
    using index_t = std::uintptr_t;

    // segmented container alias
    template <class T>
    using segmented_t = SegmentedContainer<typename T::resource_t>;
}


#endif

// end of file
