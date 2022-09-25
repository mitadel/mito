#include <mito/utilities.h>

#include <cassert>
#include <iostream>

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
    mito::utilities::segmented_t<Simplex, 3> vector;

    // assert that the container is empty and with no capacity
    assert(vector.capacity() == 0);
    assert(vector.size() == 0);

    // insert three simplices in the container
    const auto & simplex0 = vector.insert(0);
    const auto & simplex1 = vector.insert(1);
    const auto & simplex2 = vector.insert(2);

    // assert that the container has 3 elements and its capacity is also 3
    assert(vector.capacity() == 3);
    assert(vector.size() == 3);

    // erase one simplex
    vector.erase(simplex1);

    // assert that the container has 2 elements and its capacity is still 3
    assert(vector.capacity() == 3);
    assert(vector.size() == 2);

    // insert one simplex
    const auto & simplex3 = vector.insert(3);

    // assert that the container has again 3 elements and its capacity is 3
    assert(vector.capacity() == 3);
    assert(vector.size() == 3);

    // insert another simplex (trigger allocation of new segment)
    const auto & simplex4 = vector.insert(4);

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