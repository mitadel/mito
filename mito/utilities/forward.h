// code guard
#if !defined(mito_utilities_forward_h)
#define mito_utilities_forward_h


namespace mito::utilities {

    // class shared pointer
    template <class Resource, bool isConst = false, bool immortal = false>
    class SharedPointer;

    // shared pointer alias
    template <class Resource, bool immortal = false>
    using shared_ptr = SharedPointer<Resource, false, immortal>;

    // const shared pointer alias
    template <class Resource, bool immortal = false>
    using const_shared_ptr = SharedPointer<Resource, true, immortal>;

    // class segmented container
    template <class T, int N /* segment size */>
    class SegmentedContainer;

    // segmented container alias
    template <class T, int N /* segment size */>
    using segmented_t = SegmentedContainer<T, N>;
}


#endif

// end of file
