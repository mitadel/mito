// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedAllocator_h)
#define mito_utilities_SegmentedAllocator_h

// DESIGN NOTES

// Class {SegmentedAllocator} implements the memory management for a {SegmentedVector}.
//
// A {SegmentedAllocator} manages memory in multiple segments of memory. Each segment is contiguous
// in memory and consists of a memory allocation for {_segment_size} resources of type {T}. Right at
// the end of each segment, a pointer {T*} points to the beginning of the next segment, much like
// what happens in linked lists.
//
// The length of each segment, needs to be set by balancing the following trade-off:
// - a larger segment size leads to a more compact data structure with a larger number of
// elements lying adjacent to each other in memory and, in case of a fully populated segment, a
// large segment size leads to a smaller allocation time per individual resource. Note, however,
// that, in case of a non fully-populated segment, more resources than actually needed are
// committed.
// - a smaller segment size leads to a more scattered data structure and more memory allocation
// calls, but certainly reduces redundancy in memory allocation.
//
// Resources are added to this container by place-instantiating them in the next available
// location (if any). If the container is completely full, a new segment of memory is allocated
// altogether.
// The current implementation only supports immutable resources (i.e. resources can be created
// and destroyed but cannot be modified once they are into the segmented container). The use of
// {SegmentedAllocator} with a resource type {T} that is not immutable can be envisioned but has
// not been explored so far.
//
// Iterators to a {SegmentedAllocator} are smart enough to jump from one segment to the next one,
// once the end of a segment has been reached.

namespace mito::utilities {
    template <class resourceT>
    class SegmentedAllocator {
      public:
        // me
        using segmented_allocator_type = SegmentedAllocator<resourceT>;
        // my template parameter
        using resource_type = resourceT;
        // raw pointer type to unqualified resource type
        using pointer = resource_type *;
        // raw reference type to unqualified resource type
        using reference = resource_type &;
        // iterators
        using iterator = SegmentedAllocatorIterator<segmented_allocator_type>;
        using iterator_const_reference = const iterator &;

      private:
        // unqualified resource type (for internal book-keeping)
        using unqualified_resource_type = typename std::remove_const<resource_type>::type;
        // raw pointer type to unqualified resource type
        using unqualified_pointer = unqualified_resource_type *;
        // TOFIX: remove?
        // const raw pointer type to unqualified resource type
        using const_pointer = const unqualified_resource_type *;
        // TOFIX: remove?
        // const raw reference type to unqualified resource type
        using const_reference = const unqualified_resource_type &;

      public:
        // default constructor (empty data structure)
        SegmentedAllocator(int segment_size) :
            _segment_size(segment_size),
            _begin(nullptr),
            _end(_begin),
            _end_allocation(_begin),
            _n_segments(0),
            _n_elements(0)
        {}

        //  move constructor
        SegmentedAllocator(SegmentedAllocator && other) :
            _segment_size(other._segment_size),
            _begin(other._begin),
            _end(other._end),
            _end_allocation(other._end_allocation),
            _n_segments(other._n_segments),
            _n_elements(other._n_elements),
            _available_locations(std::move(other._available_locations))
        {
            // invalidate the source
            other._begin = nullptr;
            other._end = other._begin;
            other._end_allocation = other._begin;
            other._n_segments = 0;
            other._n_elements = 0;
        }

        // destructor
        ~SegmentedAllocator()
        {
            if (_begin == nullptr)
                return;

            // start from the beginning of the first segment
            unqualified_pointer ptr = _begin;

            // while the current segment is not the last segment
            while (ptr + _segment_size != _end_allocation) {
                // retrieve the location of the next segment which is left behind
                // by the segmented container right at the end of the current segment
                unqualified_pointer next =
                    *(reinterpret_cast<unqualified_pointer *>(ptr + _segment_size));

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

      public:
        inline auto capacity() const -> int
        {
            // the number of segments times the size of each segment
            return _n_segments * _segment_size;
        }

        inline auto size() const -> int { return _n_elements; }

        inline auto segment_size() const noexcept -> int { return _segment_size; }

      private:
        auto _allocate_new_segment() -> unqualified_pointer
        {
            // allocate a new segment of memory
            unqualified_pointer segment = static_cast<unqualified_pointer>(::operator new(
                _segment_size * sizeof(resource_type) + sizeof(unqualified_pointer)));
            // if it is the first segment
            if (_begin == _end) {
                // point {_begin} to the beginning of the allocated memory
                _begin = segment;
            }
            // otherwise
            else {
                // reinterpret the element after the last element in data as a {pointer} *
                unqualified_pointer * tail =
                    reinterpret_cast<unqualified_pointer *>(_end_allocation);
                // leave behind a pointer with the location of the next segment
                *tail = segment;
            }
            // increment the number of segments
            ++_n_segments;
            // update the end of the container
            _end = segment;
            // update the end of the memory allocation
            _end_allocation = segment + _segment_size;
            // return the address of the new segment of memory
            return segment;
        }

        auto _next_available_location() -> unqualified_pointer
        {
            // if there are available locations to spare
            if (!_available_locations.empty()) {
                // get an available location from the queue
                unqualified_pointer location = _available_locations.front();

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
        auto location_for_placement() -> unqualified_pointer
        {
            // fetch the next available location where to write the new element
            auto location = _next_available_location();

            // if I do not have room for the new element
            if (location == nullptr) {
                // allocate a new segment of memory
                location = _allocate_new_segment();
            }

            // if we have written past the last element
            if (location == _end) {
                // move the end of the container
                ++_end;
            }

            // all done
            return location;
        }

        // insert an element in the container
        // (increment the number of elements and remove the address of the element from the pile of
        // the available locations for reuse)
        auto insert(resource_type * element) -> iterator
        {
            // increment the size of the container
            ++_n_elements;

            // if there are available locations to spare
            if (!_available_locations.empty()) {
                // assert that the newly inserted element was inserted at the front of the queue
                assert(const_cast<pointer>(element) == _available_locations.front());
                // remove the next available location from the queue
                _available_locations.pop();
            }

            // all done
            return iterator(
                element /* ptr */, _begin + _segment_size /* segment_end */, _segment_size,
                _end /* end */);
        }

        // erase an element from the container
        // (decrement the number of elements and add the address of the element to the pile of the
        // available locations for reuse)
        auto erase(resource_type * element) -> void
        {
            // decrement the number of elements
            --_n_elements;

            // add the address of the element to the queue of the available locations for write
            _available_locations.push(const_cast<unqualified_pointer>(element));

            // all done
            return;
        }

      public:
        /**
         * iterators
         */
        constexpr auto begin() const -> iterator
        {
            // get an iterator to the first element
            return iterator(
                _begin /* ptr */, _begin + _segment_size /* segment_end */, _segment_size,
                _end /* end */);
        }

        constexpr auto end() const -> iterator
        {
            // assert that {_end} is in the last allocated segment
            assert(_end_allocation - _end <= _segment_size);
            // make an {iterator} that points to the end of my segmented container
            return iterator(
                _end /* ptr */, _end_allocation /* segment_end */, _segment_size, _end /* end */);
        }

      private:
        // delete copy constructor
        SegmentedAllocator(const SegmentedAllocator &) = delete;

        // delete assignment operator
        SegmentedAllocator & operator=(const SegmentedAllocator &) = delete;

        // delete move assignment operator
        SegmentedAllocator & operator=(SegmentedAllocator &&) = delete;

        // the segment size
        const int _segment_size;
        // the beginning of the container
        unqualified_pointer _begin;
        // the end of the container (no element has been constructed so far after this point)
        // (it does not necessarily coincide with the end of the last allocated segment)
        unqualified_pointer _end;
        // points right after the last segment of allocated memory
        unqualified_pointer _end_allocation;
        // the number of segments stored in the container
        int _n_segments;
        // the number of elements stored in the container
        int _n_elements;
        // a queue with the available locations for writing
        std::queue<unqualified_pointer> _available_locations;

      private:
        // non-const iterator
        friend iterator;
    };
}


#endif

// end of file
