// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_RepositoryIterator_h)
#define mito_utilities_RepositoryIterator_h


namespace mito::utilities {

    template <class RepositoryT>
    class RepositoryIterator {
        // types
      public:
        // my template parameters
        using repository_type = RepositoryT;
        // me
        using iterator = RepositoryIterator<repository_type>;
        // a reference to me
        using iterator_reference = iterator &;
        // what I point to
        using shared_pointer = typename repository_type::shared_ptr_t;

        // the segmented container type
        using segmented_type = typename repository_type::resource_collection_t;

        // a reference to an iterator on the segmented container
        using segmented_iterator_type = segmented_type::iterator;
        using segmented_iterator_const_reference_type = segmented_type::iterator_const_reference;

        // metamethods
      public:
        // constructor
        constexpr RepositoryIterator(segmented_iterator_const_reference_type segmented_iterator) :
            _segmented_iterator(segmented_iterator)
        {}

        // iterator protocol
      public:
        // dereference
        constexpr auto operator*() const -> shared_pointer
        {
            // wrap the resource in a shared pointer and return it
            return shared_pointer(_segmented_iterator.ptr());
        }

        // arithmetic: prefix
        constexpr auto operator++() -> iterator_reference
        {
            // increment the iterator to the segmented container
            ++_segmented_iterator;

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
        // a segmented container iterator
        segmented_iterator_type _segmented_iterator;

        // default metamethods
      public:
        // destructor
        ~RepositoryIterator() = default;
        // let the compiler write the rest
        constexpr RepositoryIterator(const RepositoryIterator &) = default;
        constexpr RepositoryIterator(RepositoryIterator &&) noexcept = default;
        constexpr RepositoryIterator & operator=(const RepositoryIterator &) = default;
        constexpr RepositoryIterator & operator=(RepositoryIterator &&) noexcept = default;
    };

    // the global operators
    // equality
    template <class RepositoryT>
    constexpr auto operator==(
        const RepositoryIterator<RepositoryT> & it1,
        const RepositoryIterator<RepositoryT> & it2) noexcept -> bool
    {
        // iterators are equal if they point to the same thing
        return *it1 == *it2;
    }

    // and not
    template <class RepositoryT>
    constexpr auto operator!=(
        const RepositoryIterator<RepositoryT> & it1,
        const RepositoryIterator<RepositoryT> & it2) noexcept -> bool
    {
        // iterators are unequal iff they are not equal
        return !(it1 == it2);
    }
}
#endif

// end of file