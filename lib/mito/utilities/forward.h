// code guard
#if !defined(mito_utilities_forward_h)
#define mito_utilities_forward_h


namespace mito::utilities {

    // base class for a reference counted object
    class Shareable;

    // concept for a reference counted object
    template <typename T>
    concept ReferenceCountedObject = std::is_base_of<Shareable, T>::value;

    // class shared pointer based on a reference counted resource
    template <class Resource>
    requires ReferenceCountedObject<Resource>
    class SharedPointer;

    // shared pointer alias
    template <class Resource>
    using shared_ptr = SharedPointer<Resource>;

    // class segmented container
    template <class T, int N /* segment size */>
    requires(
        std::is_member_function_pointer_v<decltype(&T::invalidate)>
        && std::is_member_function_pointer_v<decltype(&T::is_valid)>)
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
