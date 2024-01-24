#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/utilities.h>


class Resource : public mito::utilities::Invalidatable {
  public:
    Resource(int foo) : _foo(foo) {}

    int foo() const { return _foo; }

  private:
    int _foo;
};

// the resource type
using resource_t = Resource;


TEST(Utilities, RepositoryIterator)
{
    // instantiate a segmented vector of {resource_t} resources
    mito::utilities::segmented_vector_t<resource_t> collection(3 /*segment size */);

    // assert that the container is empty and with no capacity
    EXPECT_EQ(collection.capacity(), 0);
    EXPECT_EQ(std::size(collection), 0);

    // emplace three simplices in the container
    auto simplex0 = collection.emplace_back(0);
    auto simplex1 = collection.emplace_back(1);
    auto simplex2 = collection.emplace_back(2);

    std::vector<int> store_elements;
    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 0);
    EXPECT_EQ(store_elements[1], 1);
    EXPECT_EQ(store_elements[2], 2);
    EXPECT_EQ(std::size(store_elements), 3);

    store_elements.clear();

    // erase element in the middle
    collection.erase(simplex1);

    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 0);
    EXPECT_EQ(store_elements[1], 2);
    EXPECT_EQ(std::size(store_elements), 2);

    store_elements.clear();

    // erase first element
    collection.erase(simplex0);

    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(store_elements[0], 2);
    EXPECT_EQ(std::size(store_elements), 1);

    store_elements.clear();

    // erase last element
    collection.erase(simplex2);

    // loop on an empty repository
    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    EXPECT_EQ(std::size(store_elements), 0);

    store_elements.clear();

    // emplace 5 reusing the slot of the first erased simplex (1)
    auto simplex5 = collection.emplace_back(5);
    // emplace 6 reusing the slot of the first erased simplex (0)
    auto simplex6 = collection.emplace_back(6);
    // emplace 7 reusing the slot of the first erased simplex (2)
    auto simplex7 = collection.emplace_back(7);

    // emplace another simplex (trigger allocation of new segment)
    auto simplex4 = collection.emplace_back(4);

    for (const auto & el : collection) {
        store_elements.emplace_back(el->foo());
    }

    // the order of the values depends on the order elimination of the previous simplices
    EXPECT_EQ(store_elements[0], 6);
    EXPECT_EQ(store_elements[1], 5);
    EXPECT_EQ(store_elements[2], 7);
    EXPECT_EQ(store_elements[3], 4);
    EXPECT_EQ(std::size(store_elements), 4);
}
