#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <cmath>

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

    T * _next_available_location() const
    {
        // if the container is empty
        if (_data.size() == 0) {
            // there is no available location
            return nullptr;
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
    void add(Args &&... args)
    {
        // fetch the next available location where to write the new element
        auto location = _next_available_location();

        // if I do not have room for the new element
        if (location == nullptr) {
            // allocate a new segment of memory
            location = _allocate_new_segment();
        }

        // create a new instance of T at {location} with placement new
        new (&location) T(args...);

        // increment the size of the container
        ++_n_elements;

        // increment the index of the furthest element
        ++_end;

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

    SegmentedContainer<Simplex> vector;
    std::cout << "vector.capacity(): " << vector.capacity() << std::endl;
    std::cout << "vector.size(): " << vector.size() << std::endl;
    vector.add(5);

    std::cout << "vector.capacity(): " << vector.capacity() << std::endl;
    std::cout << "vector.size(): " << vector.size() << std::endl;

    return 0;
}