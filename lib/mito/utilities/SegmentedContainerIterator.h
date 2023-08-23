// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedContainerIterator_h)
#define mito_utilities_SegmentedContainerIterator_h


namespace mito::utilities {

    template <class SegmentedContainerT>
    class SegmentedContainerIterator {
        // types
      public:
        // my template parameters
        using segmented_container_type = SegmentedContainerT;
        // me
        using iterator = SegmentedContainerIterator<segmented_container_type>;
        using iterator_reference = iterator &;
        // what I point to
        using pointer = typename SegmentedContainerT::pointer;
        using reference = typename SegmentedContainerT::reference;

        // metamethods
      public:
        // constructor
        constexpr SegmentedContainerIterator(
            pointer ptr, pointer segment_end, int segment_size, pointer end) :
            _ptr(ptr),
            _segment_end(segment_end),
            _segment_size(segment_size),
            _end(end)
        {}

        // iterator protocol
      public:
        // dereference
        constexpr auto operator*() const -> reference
        {
            // easy enough
            return *_ptr;
        }

        // accessors
        constexpr auto ptr() const noexcept -> pointer
        {
            // easy enough
            return _ptr;
        }

        // operator->
        constexpr auto operator->() const noexcept -> pointer
        {
            // return the pointer
            return ptr();
        }

        // arithmetic: prefix
        constexpr auto operator++() -> iterator_reference
        {
            // if not at the end of the segment
            while (++_ptr) {
                // end of the container
                if (_ptr == _end) {
                    return *this;
                }

                // end of the segment
                if (_ptr == _segment_end) {
                    // retrieve the location of the next segment which is left behind
                    // by the segmented container right at the end of the current segment
                    _ptr = *(reinterpret_cast<const pointer *>(_ptr));
                    // store the start of the current segment
                    _segment_end = _ptr + _segment_size;
                }

                // if the element is valid
                if (_ptr->is_valid()) {
                    // found it
                    return *this;
                }
            }

            // you should never end up here
            assert(false);

            // all done
            return *this;
        }

        // arithmetic: postfix
        constexpr auto operator++(int) -> iterator
        {
            // make a copy of me
            auto clone = *this;
            // increment me
            ++(*this);
            // and return the clone
            return clone;
        }

        // implementation details: data
      private:
        // pointer to an element of the segments
        pointer _ptr;
        // pointer to the end of the current segment
        pointer _segment_end;
        // the size of a segment
        const int _segment_size;
        // pointer to the end of the segmented container
        // (this is necessary as the end of the container may not coincide with the end of the
        // allocated memory)
        pointer _end;

        // default metamethods
      public:
        // destructor
        ~SegmentedContainerIterator() = default;
        // let the compiler write the rest
        constexpr SegmentedContainerIterator(const SegmentedContainerIterator &) = default;
        constexpr SegmentedContainerIterator(SegmentedContainerIterator &&) noexcept = default;
        constexpr SegmentedContainerIterator & operator=(const SegmentedContainerIterator &) =
            default;
        constexpr SegmentedContainerIterator & operator=(SegmentedContainerIterator &&) noexcept =
            default;
    };

    // the global operators
    // equality
    template <class SegmentedContainerT>
    constexpr auto operator==(
        const SegmentedContainerIterator<SegmentedContainerT> & it1,
        const SegmentedContainerIterator<SegmentedContainerT> & it2) noexcept -> bool
    {
        // iterators are equal if they point to the same segmented container
        return it1.ptr() == it2.ptr();
    }

    // and not
    template <class SegmentedContainerT>
    constexpr auto operator!=(
        const SegmentedContainerIterator<SegmentedContainerT> & it1,
        const SegmentedContainerIterator<SegmentedContainerT> & it2) noexcept -> bool
    {
        // iterators are unequal iff they are not equal
        return !(it1 == it2);
    }
}
#endif

// end of file