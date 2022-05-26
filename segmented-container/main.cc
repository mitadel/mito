#include <iostream>
#include <vector>
#include <array>
#include <numeric>
#include <cmath>

template <class T, int N = 10 /* segment size */>
class SegmentedContainer {

  public:
    SegmentedContainer() : _data() {}

    ~SegmentedContainer()
    {
        for (const auto & segment : _data) {
            delete[] segment;
        }
    }

    int capacity()
    {
        // the number of segments times the size of each segment
        return _data.size() * N;
    }

    int size() { return _n_elements; }

  private:
    T * _next_available_location()
    {
        // get the offset of the furthest element with respect to a multiple of N
        int offset = _n_max % N;

        // if the size of the container is a multiple of N
        if (offset == 0) {
            // the container is all filled up, there is no available location
            return nullptr;
        }

        // the next available location is the offset applied to the beginning of the last segment
        return _data[_data.size() - 1] + offset;
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
            T * segment = static_cast<T *>(::operator new(N * sizeof(T)));
            // add the new segment to the pile
            _data.push_back(segment);
            // store the address of the new segment of memory in {location}
            location = segment;
        }

        // create a new instance of T at {location} with placement new
        new (&location) T(args...);

        // increment the size of the container
        ++_n_elements;

        // increment the index of the furthest element
        ++_n_max;

        // all done
        return;
    }

  private:
    // the underlying data
    std::vector<T *> _data;
    // the index of the furthest element in the container
    int _n_max;
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