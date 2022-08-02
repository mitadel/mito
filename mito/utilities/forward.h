// code guard
#if !defined(mito_utilities_forward_h)
#define mito_utilities_forward_h


namespace mito::utilities {

    // class segmented container
    template <class T, int N /* segment size */>
    class SegmentedContainer;

    // segmented container alias
    template <class T, int N /* segment size */>
    using segmented_t = SegmentedContainer<T, N>;
}


#endif

// end of file
