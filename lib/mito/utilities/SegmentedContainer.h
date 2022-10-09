// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedContainer_h)
#define mito_utilities_SegmentedContainer_h


namespace mito::utilities {
    template <class T, int N = 10 /* segment size */>
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
        SegmentedContainer() : _data(), _end(nullptr), _n_elements(0) {}

        // destructor
        ~SegmentedContainer()
        {
            for (const auto & segment : _data) {
                ::operator delete(segment);
            }
        }

        inline auto capacity() const -> int
        {
            // the number of segments times the size of each segment
            return _data.size() * N;
        }

        inline auto size() const -> int { return _n_elements; }

      private:
        auto _allocate_new_segment() -> T *
        {
            // allocate a new segment of memory
            T * segment = static_cast<T *>(::operator new(N * sizeof(T) + sizeof(T *)));
            // if it is not the first segment
            if (_data.size() > 0) {
                // reinterpret the element after the last element in data as a T*
                T ** tail = reinterpret_cast<T **>(_data.back() + N);
                // leave behind a pointer with the location of the next segment
                *tail = segment;
            }
            // add the new segment to the pile
            _data.push_back(segment);
            // update the end of the container
            _end = _data[_data.size() - 1];
            // return the address of the new segment of memory
            return segment;
        }

        auto _next_available_location() -> T *
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

        // TOFIX: bug if the last valid element happens before the last segment
        constexpr auto end() -> iterator
        {
            // make an {iterator} that points to the end of my segmented container
            return iterator(
                _end /* ptr */, _data[_data.size() - 1] + N /* segment_end */, _end /* end */);
        }

        constexpr auto begin() const -> const_iterator { return const_iterator(*this); }

        constexpr auto end() const -> const_iterator
        {
            // make an {iterator} that points to the end of my segmented container
            return const_iterator(*this, _data[_data.size() - 1] + N /* segment_end */, _end);
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

      private:
        // non-const iterator
        friend iterator;
        // const iterator
        friend const_iterator;
    };
}


#endif

// end of file
