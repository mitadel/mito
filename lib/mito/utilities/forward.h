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

    // class segmented allocator
    template <class resourceT>
    class SegmentedAllocator;

    // and its iterator
    template <class SegmentedAllocatorT>
    class SegmentedAllocatorIterator;

    // class segmented container iterator
    template <class SegmentedContainerT>
    class SegmentedContainerIterator;

    // the repository
    template <class sharedPointerT>
    // requires ReferenceCountedObject<typename sharedPointerT::resource_type>
    class Repository;

}


#endif

// end of file
