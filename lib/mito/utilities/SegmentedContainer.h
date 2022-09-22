// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedContainer_h)
#define mito_utilities_SegmentedContainer_h


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
            _segmentedContainer(segmentedContainer)
        {
            _ptr = _segmentedContainer.begin();
        }

        // iterator protocol
      public:
        // arithmetic: prefix
        constexpr auto operator++() -> iterator_reference
        {
            // TOFIX Add concept base type is invalidatable
            ++_ptr;
            if (_ptr == _segmentedContainer.end()) {
                return *this;
            }

            if (!_ptr->is_valid()) {
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
            ++(*this);
            // and return the clone
            return clone;
        }

        // accessors
        constexpr auto segmented_container() const -> segmented_container_const_reference
        {
            // easy enough
            return _segmentedContainer;
        }

        // implementation details: data
      private:
        segmented_container_reference _segmentedContainer;
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
        return &it1.segmented_container() == &it2.segmented_container();
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

    template <class T, int N = 10 /* segment size */>
    class SegmentedContainer {

      public:
        // aliases for my template parameters
        using segment_type = T;

        // me
        using segmented_container_type = SegmentedContainer<segment_type, N>;

        // my value
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using const_reference = const T &;

        // iterators
        using iterator = SegmentedContainerIterator<segmented_container_type, false>;
        // using const_iterator = SegmentedContainerIterator<segmented_container_type, true>;

        // default constructor (empty data structure)
        SegmentedContainer() : _data(), _end(nullptr), _n_elements(0) {}

        // destructor
        ~SegmentedContainer()
        {
            for (const auto & segment : _data) {
                ::operator delete(segment);
            }
        }

        int capacity() const
        {
            // the number of segments times the size of each segment
            return _data.size() * N;
        }

        T * end() const { return _end; }

        int size() const { return _n_elements; }

      private:
        T * _allocate_new_segment()
        {
            // allocate a new segment of memory
            T * segment = static_cast<T *>(::operator new(N * sizeof(T)));
            // add the new segment to the pile
            _data.push_back(segment);
            // update the end of the container
            _end = _data[_data.size() - 1];
            // return the address of the new segment of memory
            return segment;
        }

        T * _next_available_location()
        {
            // if the container is empty
            if (_data.size() == 0) {
                // there is no available location
                return nullptr;
            }

            if (!_available_locations.empty()) {
                // get an available location from the queue
                T * location = _available_locations.front();

                // remove the next available location from the queue
                // (TOFIX: what if the next available location is then not used?
                // Maybe we should not pop here)
                _available_locations.pop();

                // return the available location from the queue
                return location;
            }

            // if the container is all filled up
            if (_end == _data[_data.size() - 1] + N) {
                // there is no available location
                return nullptr;
            }

            // otherwise the next available location is given by {_end}
            return _end;
        }

      public:
        template <class... Args>
        auto add(Args &&... args)
        {
            // fetch the next available location where to write the new element
            auto location = _next_available_location();

            // if I do not have room for the new element
            if (location == nullptr) {
                // allocate a new segment of memory
                location = _allocate_new_segment();
            }

            // create a new instance of T at {location} with placement new
            // and assign it to a new pointer
            mito::utilities::shared_ptr<T, true> pointer(args..., location);

            // increment the size of the container
            ++_n_elements;

            // if we have written past the last element
            if (location == _end) {
                // move the end of the container
                ++_end;
            }

            // all done
            return pointer;
        }

        void erase(const mito::utilities::shared_ptr<T, true> & element)
        {
            // mark element as invalid
            element->invalidate();

            // decrement the number of elements
            --_n_elements;

            // add the address of the element to the queue of the available locations for write
            _available_locations.push(element);

            // all done
            return;
        }

        /**
         * iterators
         */
        constexpr auto begin() -> iterator
        {
            // make an {iterator} that points to the first valid data of my segmented container
            for (size_t i = 0; i < std::size(_data); ++i) {
                if (_data[i]->is_valid()) {
                    return _data.begin() + i;
                }
            }
            return _end;
        }

        constexpr auto end() -> iterator
        {
            // make an {iterator} that points to the end of my segmented container
            return iterator(*this);
        }

      private:
        // the underlying data
        std::vector<T *> _data;
        // the end of the container
        T * _end;
        // the number of elements stored in the container
        int _n_elements;
        // a queue with the available locations for writing
        std::queue<T *> _available_locations;
    };
}


#endif

// end of file
