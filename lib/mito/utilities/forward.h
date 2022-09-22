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

    // and its iterator
    template <class SegmentedContainerT, bool isConst>
    class SegmentedContainerIterator;

    // segmented container alias
    template <class T, int N /* segment size */>
    using segmented_t = SegmentedContainer<T, N>;

    // segmented container iterator
    // equality
    template <class SegmentedContainerT, bool isConst>
    constexpr auto operator==(
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it1,
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it2) -> bool;


    // and not
    template <class SegmentedContainerT, bool isConst>
    constexpr auto operator!=(
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it1,
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it2) -> bool;
}


#endif

// end of file
