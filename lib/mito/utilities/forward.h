// code guard
#if !defined(mito_utilities_forward_h)
#define mito_utilities_forward_h


namespace mito::utilities {

    // base class for a reference counted object
    class Shareable;

    // concept for a reference counted object
    template <typename resourceT>
    concept ReferenceCountedObject = std::is_base_of<Shareable, resourceT>::value;

    // class shared pointer based on a reference counted resource
    template <class resourceT>
    // requires ReferenceCountedObject<resourceT>
    class SharedPointer;

    // class segmented container
    template <class sharedResourceT>
    // requires ReferenceCountedObject<typename sharedResourceT::resource_t>
    class SegmentedContainer;

    // and its iterator
    template <class SegmentedContainerT>
    class SegmentedContainerIterator;

    // the repository
    template <class sharedResourceT>
    // requires ReferenceCountedObject<typename sharedResourceT::resource_t>
    class Repository;

    // TOFIX: move these guys to a {utilities.h} header?
    // segmented container iterator
    // equality
    template <class SegmentedContainerT>
    constexpr auto operator==(
        const SegmentedContainerIterator<SegmentedContainerT> & it1,
        const SegmentedContainerIterator<SegmentedContainerT> & it2) noexcept -> bool;


    // and not
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
