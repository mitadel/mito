// code guard
#if !defined(mito_utilities_utilities_h)
#define mito_utilities_utilities_h


namespace mito::utilities {
    // segmented container iterator equality
    template <class SegmentedContainerT>
    constexpr auto operator==(
        const SegmentedContainerIterator<SegmentedContainerT> & it1,
        const SegmentedContainerIterator<SegmentedContainerT> & it2) noexcept -> bool;


    // and inequality
    template <class SegmentedContainerT>
    constexpr auto operator!=(
        const SegmentedContainerIterator<SegmentedContainerT> & it1,
        const SegmentedContainerIterator<SegmentedContainerT> & it2) noexcept -> bool;

    // hash function for shared pointers
    // Note that two pointers pointing to the same cell collapse on the same hashed value
    template <class sharedPointerT>
    struct hash_function {
        size_t operator()(const sharedPointerT & item) const
        {
            // reinterpret the address of the pointed handle as a {size_t} and return it
            return reinterpret_cast<size_t>(item.id());
        }
    };
}


#endif

// end of file
