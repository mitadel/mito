// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedContainerIterator_h)
#define mito_utilities_SegmentedContainerIterator_h


namespace mito::utilities {
    // polymorphic base class for building iterators
    template <class containerT, bool isConst>
    class iterator_base {
      public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = typename containerT::value_type;
        using pointer = std::conditional_t<
            isConst, typename containerT::const_pointer, typename containerT::pointer>;
        using reference = std::conditional_t<
            isConst, typename containerT::const_reference, typename containerT::reference>;
    };

    template <class SegmentedContainerT, bool isConst>
    class SegmentedContainerIterator : public iterator_base<SegmentedContainerT, isConst> {
        // types
      public:
        // my template parameters
        using segmented_container_type = SegmentedContainerT;
        constexpr static int segmented_container_segment_size = SegmentedContainerT::segment_size;
        // me
        using iterator = SegmentedContainerIterator<segmented_container_type, isConst>;
        using iterator_reference = iterator &;
        // my base class
        using iterbase = iterator_base<segmented_container_type, isConst>;
        // my parts
        using segmented_container_reference = std::conditional_t<
            isConst, const segmented_container_type &, segmented_container_type &>;
        using segmented_container_const_reference = const segmented_container_type &;
        // what i point to
        using value_type = typename iterbase::value_type;
        using pointer = typename iterbase::pointer;
        using reference = typename iterbase::reference;

        // metamethods
      public:
        // constructor
        constexpr SegmentedContainerIterator(segmented_container_reference segmentedContainer) :
            _segmentedContainer(segmentedContainer),
            _index(0),
            _ptr(segmentedContainer._data[0])
        {
            // if the first element is not a valid one
            if (!_ptr->is_valid()) {
                // find the first valid element
                operator++();
            }

            // all done
            return;
        }

        // constructor
        constexpr SegmentedContainerIterator(
            segmented_container_reference segmentedContainer, size_t index,
            typename SegmentedContainerT::pointer ptr) :
            _segmentedContainer(segmentedContainer),
            _index(index),
            _ptr(ptr)
        {}

        // iterator protocol
      public:
        // dereference
        constexpr auto operator*() const -> reference
        {
            // retrieve the value and return it
            return *_ptr;
        }

        // arithmetic: prefix
        constexpr auto operator++() -> iterator_reference
        {
            // TOFIX Add concept base type is invalidatable

            const auto & data = _segmentedContainer._data;

            // loop on segments
            for (; _index < data.size(); ++_index) {
                // if not at the end of the segment
                while (_ptr != data[_index] + segmented_container_segment_size) {
                    // try next element
                    ++_ptr;
                    // if the element is valid
                    if (_ptr->is_valid()) {
                        // found it
                        return *this;
                    }
                }
            }

            // restore index to last segment
            --_index;

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

        // accessors
        constexpr auto ptr() const -> typename SegmentedContainerT::pointer
        {
            // easy enough
            return _ptr;
        }

        // implementation details: data
      private:
        // reference to segmented container
        segmented_container_reference _segmentedContainer;
        // index for segments
        size_t _index;
        // pointer to an element of the segments
        typename SegmentedContainerT::pointer _ptr;

        // default metamethods
      public:
        // destructor
        ~SegmentedContainerIterator() = default;
        // let the compiler write the rest
        constexpr SegmentedContainerIterator(const SegmentedContainerIterator &) = default;
        constexpr SegmentedContainerIterator(SegmentedContainerIterator &&) = default;
        constexpr SegmentedContainerIterator & operator=(const SegmentedContainerIterator &) =
            default;
        constexpr SegmentedContainerIterator & operator=(SegmentedContainerIterator &&) = default;
    };

    // the global operators
    // equality
    template <class SegmentedContainerT, bool isConst>
    constexpr auto operator==(
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it1,
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it2) -> bool
    {
        // iterators are equal if they point to the same segmented container
        return it1.ptr() == it2.ptr();
    }

    // and not
    template <class SegmentedContainerT, bool isConst>
    constexpr auto operator!=(
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it1,
        const SegmentedContainerIterator<SegmentedContainerT, isConst> & it2) -> bool
    {
        // iterators are unequal iff they are not equal
        return !(it1 == it2);
    }
}
#endif

// end of file