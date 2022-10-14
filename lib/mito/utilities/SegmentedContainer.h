// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedContainer_h)
#define mito_utilities_SegmentedContainer_h

// DESIGN NOTES

// Class {SegmentedContainer} implements a data structure that is resizable, while also being stable 
// in memory and able to concurrently instantiate multiple resources collectively. 
// While this data structure has been developed with the application in mind of collecting mesh 
// elements, the implementation is quite general and can be extended to storage of other type of 
// resources.

// A {SegmentedContainer} is articulated in multiple segments of (contiguous) memory. Each segment
// allocates memory for {N} resources of type {T}. Right at the end of each segment, a pointer {T*} 
// allows to reach the beginning of the next segment, much like what happens in linked lists. 

// Resources are added to this container by place-instantiating them in the next available 
// location (if any). If the container is completely full, a new segment of memory is allocated 
// altogether. 
// Resources are removed from this container by marking the resource as 'invalid' and signing up
// its position in memory to be overwritten by a new resource.

// The template parameter {N}, namely the length of each segment, needs to be set by balancing the
// following trade-off: 
// - a higher value of {N} leads to a more compact data structure with a larger number of elements 
// lying adjacent to each other in memory and, in case of a fully populated segment, a high value 
// of {N} leads to a smaller allocation time per individual resource. Note, however, that, in case 
// of a non fully-populated segment, more resources than actually needed are committed.
// - a smaller value of {N} leads to a more scattered data structure and more memory allocation 
// calls, but certainly reduces redundancy in memory allocation.

// Iterators to this data structure are smart enough to skip the invalid elements and jump from one 
// segment to the next one, once the end of a segment has been reached.

// The {SegmentedContainer} data structure is meant to closely collaborate with a {SharedPointer} 
// class, which provides the information of resources being unused and therefore redundant. However, 
// the {SharedPointer} class differs from the {shared_ptr} offered by the C++ standard library in 
// two respects: 
//      1) the {SharedPointer} does not have ownership of the resource and does not have privileges 
//          to allocate and deallocate memory for it. In fact, the allocation and deallocation of 
//          memory is entirely managed by the {SegmentedContainer} class, which allocates and 
//          deallocates resources collectively to optimize memory allocation/deallocation. In this 
//          respect, the role of the {SharedPointer} is only that of managing the book keeping on 
//          the count of references to the resources.
//      2) the {SharedPointer} does not store the reference count directly, but manages a reference 
//          count stored by the resource. The reason for this is that, in order not to compromise 
//          the correctness of the reference count, the {SegmentedContainer} should only make the 
//          resource available to its clients via a {SharedPointer}, which knows how to do the 
//          relative book keeping. Because the reference count is stored directly within the 
//          resource, the {SegmentedContainer} does not need to store a {SharedPointer} to each of 
//          its resources but is able to synthesize a {SharedPointer} on the fly, upon request of 
//          a given resource by a given client.

// The only requirement of the {SegmentedContainer} on the template type {T} of the resources stored 
// is simply that the resource is a {ReferenceCountedObject}, which means that the resource is able 
// to provide the machinery needed by the {SharedPointer} to do the book keeping. Specifically, this 
// translates into the class of the resource inheriting from class {Shareable}, which provides the 
// minimal interface necessary to collaborate with {SharedPointer} to perform reference counting.
// Ideally, the resource is otherwise immutable (i.e. except from the reference count attribute). 
// The use of {SegmentedContainer} for a class that is not immutable can be envisioned but has not 
// been explored so far.

namespace mito::utilities {
    template <class T, int N /* segment size */>
    requires ReferenceCountedObject<T>
    class SegmentedContainer {

      public:
        // aliases for my template parameters
        using resource_type = T;
        // aliases for my segment size
        constexpr static int segment_size = N;

        // me
        using segmented_container_type = SegmentedContainer<resource_type, segment_size>;

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
            T * resource = new (location) T(args...);
            // and assign it to a new pointer
            mito::utilities::shared_ptr<T> pointer(resource);

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

        auto erase(mito::utilities::shared_ptr<T> & element) -> void
        {
            // decrement the number of elements
            --_n_elements;

            // add the address of the element to the queue of the available locations for write
            _available_locations.push(element);

            // assert that you are the last one reference to this item
            assert(element.references() == 1);

            // reset the shared pointer
            element.reset();

            // all done
            return;
        }

        /**
         * iterators
         */
        constexpr auto begin() -> iterator
        {
            // get an iterator to the first element
            auto it = iterator(_begin /* ptr */, _begin + N /* segment_end */, _end /* end */);

            // if the first element is valid, return it, else return the next valid element
            return (it->is_valid() ? it : ++it);
        }

        constexpr auto end() -> iterator
        {
            // assert that {_end} is in the last allocated segment
            assert(_end_allocation - _end <= N);
            // make an {iterator} that points to the end of my segmented container
            return iterator(_end /* ptr */, _end_allocation /* segment_end */, _end /* end */);
        }

        constexpr auto begin() const -> const_iterator
        {
            // get an iterator to the first element
            auto it =
                const_iterator(_begin /* ptr */, _begin + N /* segment_end */, _end /* end */);

            // if the first element is valid, return it, else return the next valid element
            return (it->is_valid() ? it : ++it);
        }

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
