// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedContainer_h)
#define mito_utilities_SegmentedContainer_h


namespace mito::utilities {
    template <class T, int N = 10 /* segment size */>
    requires(
        std::is_member_function_pointer_v<decltype(&T::invalidate)>
        && std::is_member_function_pointer_v<decltype(&T::is_valid)>)
    class SegmentedContainer {

      public:
        // aliases for my template parameters
        using segment_type = T;
        // aliases for my segment size
        constexpr static int segment_size = N;

        // me
        using segmented_container_type = SegmentedContainer<segment_type, segment_size>;

        // my value
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using const_reference = const T &;

        // iterators
        using iterator = SegmentedContainerIterator<segmented_container_type, false /* isConst */>;
        using const_iterator =
            SegmentedContainerIterator<segmented_container_type, true /* isConst */>;

        // default constructor (empty data structure)
        SegmentedContainer() :
            _begin(nullptr),
            _end(_begin),
            _end_allocation(_begin),
            _n_segments(0),
            _n_elements(0)
        {}

        // destructor
        ~SegmentedContainer()
        {
            // start from the beginning of the first segment
            T * ptr = _begin;

            // while the current segment is not the last segment
            while (ptr + N != _end_allocation) {
                // retrieve the location of the next segment which is left behind
                // by the segmented container right at the end of the current segment
                T * next = *(reinterpret_cast<pointer *>(ptr + N));

                // delete the current segment
                ::operator delete(ptr);

                // go to the next segment
                ptr = next;
            }

            // delete the last segment
            ::operator delete(ptr);

            // all done
            return;
        }

        inline auto capacity() const -> int
        {
            // the number of segments times the size of each segment
            return _n_segments * N;
        }

        inline auto size() const -> int { return _n_elements; }

      private:
        auto _allocate_new_segment() -> T *
        {
            // allocate a new segment of memory
            T * segment = static_cast<T *>(::operator new(N * sizeof(T) + sizeof(T *)));
            // if it is the first segment
            if (_begin == _end) {
                // point {_begin} to the beginning of the allocated memory
                _begin = segment;
            }
            // otherwise
            else {
                // reinterpret the element after the last element in data as a T*
                T ** tail = reinterpret_cast<T **>(_end_allocation);
                // leave behind a pointer with the location of the next segment
                *tail = segment;
            }
            // increment the number of segments
            ++_n_segments;
            // update the end of the container
            _end = segment;
            // update the end of the memory allocation
            _end_allocation = segment + N;
            // return the address of the new segment of memory
            return segment;
        }

        auto _next_available_location() -> T *
        {
            // if there are available locations to spare
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

            // if the container is all filled up (note that this case also includes the case that
            // the container is completely empty)
            if (_end == _end_allocation) {
                // there is no available location
                return nullptr;
            }

            // otherwise the next available location is given by {_end}
            return _end;
        }

      public:
        template <class... Args>
        auto insert(Args &&... args) -> auto
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

        auto erase(const mito::utilities::shared_ptr<T, true> & element) -> void
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
        constexpr auto begin() -> iterator { return iterator(*this); }

        constexpr auto end() -> iterator
        {
            // assert that {_end} is in the last allocated segment
            assert(_end_allocation - _end <= N);
            // make an {iterator} that points to the end of my segmented container
            return iterator(_end /* ptr */, _end_allocation /* segment_end */, _end /* end */);
        }

        constexpr auto begin() const -> const_iterator { return const_iterator(*this); }

        constexpr auto end() const -> const_iterator
        {
            // assert that {_end} is in the last allocated segment
            assert(_end_allocation - _end <= N);
            // make an {iterator} that points to the end of my segmented container
            return const_iterator(
                _end /* ptr */, _end_allocation /* segment_end */, _end /* end */);
        }

      private:
        // the beginning of the container
        T * _begin;
        // the end of the container (no element has been constructed so far after this point)
        // (it does not necessarily coincide with the end of the last allocated segment)
        T * _end;
        // points right after the last segment of allocated memory
        T * _end_allocation;
        // the number of segments stored in the container
        int _n_segments;
        // the number of elements stored in the container
        int _n_elements;
        // a queue with the available locations for writing
        std::queue<T *> _available_locations;

      private:
        // non-const iterator
        friend iterator;
        // const iterator
        friend const_iterator;
    };
}


#endif

// end of file
