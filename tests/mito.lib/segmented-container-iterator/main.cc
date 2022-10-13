#include <gtest/gtest.h>
#include <iostream>
#include <mito/utilities.h>

class Simplex : public mito::utilities::Shareable {
  public:
    Simplex(int foo) : _foo(foo) {}

    int foo() const { return _foo; }

  private:
    int _foo;
};

TEST(SegmentedContainerIterator, TestSegmentedContainerIterator)
{
    // instantiate a segmented container
    mito::utilities::segmented_t<Simplex, 3> vector;

    // assert that the container is empty and with no capacity
    EXPECT_EQ(vector.capacity(), 0);
    EXPECT_EQ(vector.size(), 0);

    // insert three simplices in the container
    auto simplex0 = vector.insert(0);
    auto simplex1 = vector.insert(1);
    auto simplex2 = vector.insert(2);

    std::vector<int> store_elements;
    for (const auto & el : vector) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 0);
    EXPECT_EQ(store_elements[1], 1);
    EXPECT_EQ(store_elements[2], 2);
    EXPECT_EQ(std::size(store_elements), 3);

    store_elements.clear();

    // invalidate element in the middle
    simplex1.reset();

    for (const auto & el : vector) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 0);
    EXPECT_EQ(store_elements[1], 2);
    EXPECT_EQ(std::size(store_elements), 2);

    store_elements.clear();

    // invalidate first element
    simplex0.reset();

    for (const auto & el : vector) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 2);
    EXPECT_EQ(std::size(store_elements), 1);

    store_elements.clear();

    // invalidate last element
    simplex2.reset();

    for (const auto & el : vector) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(std::size(store_elements), 0);

    store_elements.clear();

    // insert another simplex (trigger allocation of new segment)
    auto simplex4 = vector.insert(4);

    for (const auto & el : vector) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 4);
    EXPECT_EQ(std::size(store_elements), 1);
}