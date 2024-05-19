// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


namespace mito::utilities {

    // forward declaration of segmented container iterator equality
    template <class SegmentedContainerT>
    constexpr auto operator==(
        const SegmentedContainerIterator<SegmentedContainerT> & it1,
        const SegmentedContainerIterator<SegmentedContainerT> & it2) noexcept -> bool;

    template <class SegmentedContainerT>
    class SegmentedContainerIterator {
        // types
      public:
        // my template parameters
        using segmented_container_type = SegmentedContainerT;
        // me
        using iterator = SegmentedContainerIterator<segmented_container_type>;
        // a reference to me
        using iterator_reference = iterator &;
        // what I point to
        using pointer_type = typename segmented_container_type::pointer_type;
        // the resource stored
        using resource_type = typename segmented_container_type::resource_type;

        // the segmented allocator type
        using segmented_type = typename segmented_container_type::resource_collection_type;

        // a reference to an iterator on the segmented allocator
        using segmented_iterator_type = segmented_type::iterator;
        using segmented_iterator_const_reference_type = segmented_type::iterator_const_reference;

        // metamethods
      public:
        // constructor
        constexpr SegmentedContainerIterator(
            segmented_iterator_const_reference_type segmented_iterator) :
            _segmented_iterator(segmented_iterator)
        {
            if (_segmented_iterator.ptr() != std::end(_segmented_iterator)) {
                // if you found an invalid element
                if (!_segmented_iterator->is_valid()) {
                    // move on to the next one
                    operator++();
                }
            }
        }

        // iterator protocol
      public:
        // dereference (case reference-counted object)
        constexpr auto operator*() const -> pointer_type
        requires(reference_countable_c<resource_type>)
        {
            // wrap the resource in a shared pointer and return it
            return pointer_type(_segmented_iterator.ptr());
        }

        // dereference (case non reference-counted object)
        constexpr auto operator*() const -> resource_type &
        requires(!reference_countable_c<resource_type>)
        {
            // return the resource
            return *(_segmented_iterator.ptr());
        }

        // operator->
        constexpr auto operator->() const noexcept -> pointer_type
        {
            // return the pointer
            return _segmented_iterator.ptr();
        }

        // arithmetic: prefix
        constexpr auto operator++() -> iterator_reference
        {
            // increment the iterator to the segmented allocator
            ++_segmented_iterator;

            if (_segmented_iterator.ptr() == std::end(_segmented_iterator)) {
                return *this;
            }

            // if you found an invalid element
            if (!_segmented_iterator->is_valid()) {
                // move on to the next one
                return operator++();
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
            ++_segmented_iterator;
            // and return the clone
            return clone;
        }

        // implementation details: data
      private:
        // a segmented allocator iterator
        segmented_iterator_type _segmented_iterator;

        // befriend operator==
        friend constexpr auto operator==<SegmentedContainerT>(
            const SegmentedContainerIterator<SegmentedContainerT> & it1,
            const SegmentedContainerIterator<SegmentedContainerT> & it2) noexcept -> bool;

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
        // iterators are equal if they point to the same thing
        return it1._segmented_iterator == it2._segmented_iterator;
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


// end of file