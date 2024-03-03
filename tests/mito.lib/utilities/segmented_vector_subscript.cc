#include <gtest/gtest.h>
#include <mito/base.h>
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
    collection.emplace_back(0);
    EXPECT_TRUE(collection[0].foo() == 0);

    // emplace resource in the container
    collection.emplace_back(1);
    EXPECT_TRUE(collection[1].foo() == 1);

    // emplace resource in the container
    auto resource = collection.emplace_back(2);
    EXPECT_TRUE(collection[2].foo() == 2);

    // emplace resource in the container
    collection.emplace_back(3);
    EXPECT_TRUE(collection[3].foo() == 3);

    // emplace resource in the container
    collection.emplace_back(4);
    EXPECT_TRUE(collection[4].foo() == 4);

    // emplace resource in the container
    collection.emplace_back(5);
    EXPECT_TRUE(collection[5].foo() == 5);

    // erase a resource
    collection.erase(resource);

    // check that the resource erased has been marked as invalid
    EXPECT_TRUE(collection[2].is_valid() == false);
}
