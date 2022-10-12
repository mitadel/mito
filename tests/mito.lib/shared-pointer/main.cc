#include <gtest/gtest.h>
#include <iostream>
#include <mito/utilities.h>

class Resource : public mito::utilities::Shareable {
  public:
    Resource(int a) : _a(a) { std::cout << "Built resource " << _a << std::endl; }

    ~Resource() { std::cout << "Destroying resource " << _a << std::endl; }

    int _a;
};

// the type of resource to be stored
using resource_t = Resource;

TEST(SharedPointer, TestSegmentAllocation)
{
    // the size of the segment of memory
    constexpr int N = 10;
    // the initialization value for the resource
    int a = 10;

    // allocate a new segment of memory
    resource_t * segment = static_cast<resource_t *>(::operator new(N * sizeof(resource_t)));

    // select a location within the segment of memory
    resource_t * location = (segment + 3);

    // instantiate new resource at {location}
    resource_t * resource = new (location) resource_t(a);
    mito::utilities::shared_ptr<resource_t> handle(resource);

    // modify the resource
    handle->_a += 1;

    // assert that the resource was modified correctly
    EXPECT_EQ(handle->_a, a + 1);

    // free the segment of memory
    ::operator delete(segment);
}