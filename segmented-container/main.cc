#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <cmath>
#include <queue>
#include <cassert>

template <class T, int N = 10 /* segment size */>
class SegmentedContainer {

  public:
    // default constructor (empty data structure)
    SegmentedContainer() : _data(), _end(nullptr), _n_elements(0) {}

    // destructor
    ~SegmentedContainer()
    {
        for (const auto & segment : _data) {
            delete[] segment;
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
    T * add(Args &&... args)
    {
        // fetch the next available location where to write the new element
        auto location = _next_available_location();

        // if I do not have room for the new element
        if (location == nullptr) {
            // allocate a new segment of memory
            location = _allocate_new_segment();
        }

        // create a new instance of T at {location} with placement new
        new (location) T(args...);

        // increment the size of the container
        ++_n_elements;

        // if we have written past the last element
        if (location == _end) {
            // move the end of the container
            ++_end;
        }

        // all done
        return location;
    }

    void erase(T * element)
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

class Simplex {
  public:
    Simplex(int foo) : _foo(foo), _is_valid(true) {}

    void invalidate() { _is_valid = false; }

    bool is_valid() { return _is_valid; }

  private:
    int _foo;
    bool _is_valid;
};

int
main()
{
    // instantiate a segmented container
    SegmentedContainer<Simplex, 3> vector;

    // assert that the container is empty and with no capacity
    assert(vector.capacity() == 0);
    assert(vector.size() == 0);

    // add three simplices to the container
    Simplex * simplex0 = vector.add(0);
    Simplex * simplex1 = vector.add(1);
    Simplex * simplex2 = vector.add(2);

    // assert that the container has 3 elements and its capacity is also 3
    assert(vector.capacity() == 3);
    assert(vector.size() == 3);

    // erase one simplex
    vector.erase(simplex1);

    // assert that the container has 2 elements and its capacity is still 3
    assert(vector.capacity() == 3);
    assert(vector.size() == 2);

    // add one simplex
    Simplex * simplex3 = vector.add(3);

    // assert that the container has again 3 elements and its capacity is 3
    assert(vector.capacity() == 3);
    assert(vector.size() == 3);

    // add another simplex (trigger allocation of new segment)
    Simplex * simplex4 = vector.add(4);

    // assert that the container has now 4 elements and its capacity is 6
    // (new memory allocation was in fact triggered)
    assert(vector.capacity() == 6);
    assert(vector.size() == 4);

    // erase all the simplices
    vector.erase(simplex0);
    vector.erase(simplex2);
    vector.erase(simplex3);
    vector.erase(simplex4);

    // assert that the container is empty and but has still capacity of 6
    assert(vector.capacity() == 6);
    assert(vector.size() == 0);

    // all done
    return 0;
}