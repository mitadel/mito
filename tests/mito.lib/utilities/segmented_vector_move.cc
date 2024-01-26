#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/utilities.h>


class Resource : public mito::utilities::Invalidatable {
  public:
    Resource(int foo) : _foo(foo) {}

  private:
    int _foo;
};

// the resource type
using resource_t = Resource;


TEST(Utilities, SegmentedVectorMove)
{
    // segment size
    const auto segmentSize = 3;

    // instantiate a segmented vector of {resource_t} resources
    mito::utilities::segmented_vector_t<resource_t> collection(segmentSize);

    // emplace resources in the container
    auto resource_0 = collection.emplace_back(0);
    auto resource_1 = collection.emplace_back(1);
    auto resource_2 = collection.emplace_back(2);
    auto resource_3 = collection.emplace_back(3);

    EXPECT_EQ(collection.size(), 4);
    EXPECT_EQ(collection.capacity(), 6);

    mito::utilities::segmented_vector_t<resource_t> collection2(std::move(collection));

    EXPECT_EQ(collection2.size(), 4);
    EXPECT_EQ(collection2.capacity(), 6);

    EXPECT_EQ(collection.size(), 0);
    EXPECT_EQ(collection.capacity(), 0);
}
