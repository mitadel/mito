#include <mito/utilities.h>

#include <cassert>
#include <iostream>

class Simplex {
  public:
    Simplex(int foo) : _foo(foo), _is_valid(true) {}

    void invalidate() { _is_valid = false; }

    bool is_valid() { return _is_valid; }

    int foo() const { return _foo; }

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

    // add three simplices to the container
    auto simplex0 = vector.add(0);
    auto simplex1 = vector.add(1);
    auto simplex2 = vector.add(2);

    std::vector<int> store_elements;
    for (const auto & el : vector) {
        store_elements.emplace_back(el.foo());
    }

    assert(store_elements[0] == 0);
    assert(store_elements[1] == 1);
    assert(store_elements[2] == 2);
    assert(std::size(store_elements) == 3);

    store_elements.clear();

    // invalidate element in the middle
    simplex1->invalidate();

    for (const auto & el : vector) {
        store_elements.emplace_back(el.foo());
    }

    assert(store_elements[0] == 0);
    assert(store_elements[1] == 2);
    assert(std::size(store_elements) == 2);

    store_elements.clear();

    // invalidate first element
    simplex0->invalidate();

    for (const auto & el : vector) {
        store_elements.emplace_back(el.foo());
    }

    assert(store_elements[0] == 2);
    assert(std::size(store_elements) == 1);

    store_elements.clear();

    // invalidate last element
    simplex2->invalidate();

    for (const auto & el : vector) {
        store_elements.emplace_back(el.foo());
    }

    assert(std::size(store_elements) == 0);

    store_elements.clear();

    // add another simplex (trigger allocation of new segment)
    const auto & simplex4 = vector.add(4);

    for (const auto & el : vector) {
        store_elements.emplace_back(el.foo());
    }

    assert(store_elements[0] == 4);
    assert(std::size(store_elements) == 1);

    // all done
    return 0;
}