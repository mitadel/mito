// -*- C++ -*-
//


// code guard
#if !defined(mito_utilities_SegmentedContainer_h)
#define mito_utilities_SegmentedContainer_h


namespace mito::utilities {


    template <class T, int N = 10 /* segment size */>
    class SegmentedContainer {

      public:
        // default constructor (empty data structure)
        SegmentedContainer() : _data(), _end(nullptr), _n_elements(0) {}

        // destructor
        ~SegmentedContainer()
        {
            for (const auto & segment : _data) {
                ::operator delete(segment);
            }
        }

        inline int capacity() const
        {
            // the number of segments times the size of each segment
            return _data.size() * N;
        }

        inline T * end() const { return _end; }

        inline int size() const { return _n_elements; }

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
