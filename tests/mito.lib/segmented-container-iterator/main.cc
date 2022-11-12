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
    mito::utilities::segmented_t<Simplex, 3> collection;

    // assert that the container is empty and with no capacity
    EXPECT_EQ(collection.capacity(), 0);
    EXPECT_EQ(collection.size(), 0);

    // emplace three simplices in the container
    auto simplex0 = collection.emplace(0);
    auto simplex1 = collection.emplace(1);
    auto simplex2 = collection.emplace(2);

    std::vector<int> store_elements;
    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 0);
    EXPECT_EQ(store_elements[1], 1);
    EXPECT_EQ(store_elements[2], 2);
    EXPECT_EQ(std::size(store_elements), 3);

    store_elements.clear();

    // invalidate element in the middle
    simplex1.reset();

    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 0);
    EXPECT_EQ(store_elements[1], 2);
    EXPECT_EQ(std::size(store_elements), 2);

    store_elements.clear();

    // invalidate first element
    simplex0.reset();

    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 2);
    EXPECT_EQ(std::size(store_elements), 1);

    store_elements.clear();

    // invalidate last element
    simplex2.reset();

    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(std::size(store_elements), 0);

    store_elements.clear();

    // emplace another simplex (trigger allocation of new segment)
    auto simplex4 = collection.emplace(4);

    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 4);
    EXPECT_EQ(std::size(store_elements), 1);
}