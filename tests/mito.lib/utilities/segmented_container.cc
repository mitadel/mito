#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/utilities.h>


class Resource : public mito::utilities::Shareable {
  public:
    Resource(int foo) : _foo(foo) {}

  private:
    int _foo;
};

// the resource type
using resource_t = mito::utilities::shared_ptr<Resource>;


TEST(Utilities, SegmentedContainer)
{
    // segment size
    const auto segmentSize = 3;

    // instantiate a repository of {resource_t} resources
    mito::utilities::repository_t<resource_t> collection(segmentSize);

    // check segment size
    EXPECT_EQ(collection.resources().segment_size(), segmentSize);

    // assert that the container is empty and with no capacity
    EXPECT_EQ(collection.resources().capacity(), 0);
    EXPECT_EQ(std::size(collection.resources()), 0);

    // emplace three simplices in the container
    auto simplex0 = collection.emplace(0);
    auto simplex1 = collection.emplace(1);

    // assert that the container has 2 elements and its capacity is 3
    EXPECT_EQ(collection.resources().capacity(), 3);
    EXPECT_EQ(std::size(collection.resources()), 2);

    // emplace one simplex
    auto simplex2 = collection.emplace(2);

    // assert that the container has 3 elements and its capacity is 3
    EXPECT_EQ(collection.resources().capacity(), 3);
    EXPECT_EQ(std::size(collection.resources()), 3);

    // erase it
    collection.erase(simplex2);

    // assert that the container has 2 elements and its capacity is still 3
    EXPECT_EQ(collection.resources().capacity(), 3);
    EXPECT_EQ(std::size(collection.resources()), 2);

    // emplace one simplex
    auto simplex3 = collection.emplace(3);

    // assert that the container has again 3 elements and its capacity is 3
    EXPECT_EQ(collection.resources().capacity(), 3);
    EXPECT_EQ(std::size(collection.resources()), 3);

    // emplace another simplex (trigger allocation of new segment)
    auto simplex4 = collection.emplace(4);

    // assert that the container has now 4 elements and its capacity is 6
    // (new memory allocation was in fact triggered)
    EXPECT_EQ(collection.resources().capacity(), 6);
    EXPECT_EQ(std::size(collection.resources()), 4);

    // erase more simplices
    collection.erase(simplex3);
    collection.erase(simplex4);

    // assert that the container has now 2 elements but has still capacity of 6
    EXPECT_EQ(collection.resources().capacity(), 6);
    EXPECT_EQ(std::size(collection.resources()), 2);
}
