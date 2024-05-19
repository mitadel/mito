#include <gtest/gtest.h>
#include <mito/utilities.h>


class Resource : public mito::utilities::Invalidatable {
  public:
    Resource(int foo) : _foo(foo) {}

    int foo() { return _foo; }

  private:
    int _foo;
};

// the resource type
using resource_t = Resource;


TEST(Utilities, SegmentedVectorSubscript)
{
    // segment size
    const auto segmentSize = 3;

    // instantiate a segmented vector of {resource_t} resources
    mito::utilities::segmented_vector_t<resource_t> collection(segmentSize);

    // emplace resource in the container
    collection.emplace(0);
    EXPECT_EQ(0, collection[0].foo());

    // emplace resource in the container
    collection.emplace(1);
    EXPECT_EQ(1, collection[1].foo());

    // emplace resource in the container
    auto & resource = collection.emplace(2);
    EXPECT_EQ(2, collection[2].foo());

    // emplace resource in the container
    collection.emplace(3);
    EXPECT_EQ(3, collection[3].foo());

    // emplace resource in the container
    collection.emplace(4);
    EXPECT_EQ(4, collection[4].foo());

    // emplace resource in the container
    collection.emplace(5);
    EXPECT_EQ(5, collection[5].foo());

    // erase a resource
    collection.erase(resource);

    // check that the resource erased has been marked as invalid
    EXPECT_FALSE(collection[2].is_valid());
}
