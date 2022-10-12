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

TEST(SharedPointer, DefaultConstructor)
{
    mito::utilities::Shareable shareable1;

    resource_t resource(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource);

    EXPECT_EQ(shareable1.references(), shared_ptr1.references());
}

TEST(SharedPointer, CopyConstructor)
{
    resource_t resource(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource);
    mito::utilities::SharedPointer<resource_t> shared_ptr2 = shared_ptr1;

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2 = shareable1;

    EXPECT_EQ(shareable1.references(), shared_ptr1.references());
    EXPECT_EQ(shareable2.references(), shared_ptr2.references());
}

TEST(SharedPointer, MoveConstructor)
{
    resource_t resource(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource);
    mito::utilities::SharedPointer<resource_t> shared_ptr2(std::move(shared_ptr1));

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2(std::move(shareable1));

    EXPECT_EQ(shareable2.references(), shared_ptr2.references());
}

TEST(SharedPointer, AssignmentOperator)
{
    resource_t resource1(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
    resource_t resource2(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr2(&resource2);
    shared_ptr2 = shared_ptr1;

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2;
    shareable2 = shareable1;

    EXPECT_EQ(shareable1.references(), shared_ptr1.references());
    EXPECT_EQ(shareable2.references(), shared_ptr2.references());
}

TEST(SharedPointer, MoveAssignmentOperator)
{
    resource_t resource1(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
    resource_t resource2(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr2(&resource2);
    shared_ptr2 = std::move(shared_ptr1);

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2;
    shareable2 = std::move(shareable1);

    EXPECT_EQ(shareable2.references(), shared_ptr2.references());
}

TEST(SharedPointer, ThreeWayAssignment)
{
    resource_t resource1(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
    mito::utilities::SharedPointer<resource_t> shared_ptr2 = shared_ptr1;
    resource_t resource3(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr3(&resource3);
    shared_ptr2 = shared_ptr3;

    mito::utilities::Shareable shareable1;
    mito::utilities::Shareable shareable2 = shareable1;
    mito::utilities::Shareable shareable3;
    shareable2 = shareable3;

    EXPECT_EQ(shareable1.references(), shared_ptr1.references());
    EXPECT_EQ(shareable2.references(), shared_ptr2.references());
    EXPECT_EQ(shareable3.references(), shared_ptr3.references());
}

TEST(SharedPointer, Destructor)
{
    mito::utilities::SharedPointer<resource_t> shared_ptr2;
    {
        resource_t resource1(1);
        mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
        shared_ptr2 = shared_ptr1;
    }

    mito::utilities::Shareable shareable2;
    {
        mito::utilities::Shareable shareable1;
        shareable2 = shareable1;
    }

    EXPECT_EQ(shareable2.references(), shared_ptr2.references());
}

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