// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#if !defined(mito_utilities_SegmentedAllocatorIterator_h)
#define mito_utilities_SegmentedAllocatorIterator_h


namespace mito::utilities {

    template <class SegmentedAllocatorT>
    class SegmentedAllocatorIterator {
        // types
      public:
        // my template parameters
        using segmented_allocator_type = SegmentedAllocatorT;
        // me
        using iterator = SegmentedAllocatorIterator<segmented_allocator_type>;
        using iterator_reference = iterator &;
        // what I point to
        using pointer = typename SegmentedAllocatorT::pointer;
        using reference = typename SegmentedAllocatorT::reference;

        // metamethods
      public:
        // constructor
        constexpr SegmentedAllocatorIterator(
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

        // accessors
        constexpr auto end() const noexcept -> pointer
        {
            // easy enough
            return _end;
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
            // move on to the next item
            ++_ptr;

            // if you reached the end of the container
            if (_ptr == _end) {
                // return the end
                return *this;
            }

            // if you reached the end of the segment
            if (_ptr == _segment_end) {
                // retrieve the location of the next segment (which is left behind
                // by the segmented container right at the end of the current segment)
                _ptr = *(reinterpret_cast<const pointer *>(_ptr));
                // take note of the end of the next segment
                _segment_end = _ptr + _segment_size;
            }

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
        ~SegmentedAllocatorIterator() = default;
        // let the compiler write the rest
        constexpr SegmentedAllocatorIterator(const SegmentedAllocatorIterator &) = default;
        constexpr SegmentedAllocatorIterator(SegmentedAllocatorIterator &&) noexcept = default;
        constexpr SegmentedAllocatorIterator & operator=(const SegmentedAllocatorIterator &) =
            default;
        constexpr SegmentedAllocatorIterator & operator=(SegmentedAllocatorIterator &&) noexcept =
            default;
    };

    // the global operators
    // equality
    template <class SegmentedAllocatorT>
    constexpr auto operator==(
        const SegmentedAllocatorIterator<SegmentedAllocatorT> & it1,
        const SegmentedAllocatorIterator<SegmentedAllocatorT> & it2) noexcept -> bool
    {
        // iterators are equal if they point to the same segmented container
        return it1.ptr() == it2.ptr();
    }

    // and not
    template <class SegmentedAllocatorT>
    constexpr auto operator!=(
        const SegmentedAllocatorIterator<SegmentedAllocatorT> & it1,
        const SegmentedAllocatorIterator<SegmentedAllocatorT> & it2) noexcept -> bool
    {
        // iterators are unequal iff they are not equal
        return !(it1 == it2);
    }
}
#endif

// end of file