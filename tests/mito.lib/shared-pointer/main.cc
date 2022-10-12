#include <gtest/gtest.h>
#include <mito/utilities.h>

#include <memory>

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
    auto std_shared_ptr1 = std::make_shared<int>(1);

    resource_t resource(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource);

    EXPECT_EQ(shared_ptr1.references(), std_shared_ptr1.use_count());
}

TEST(SharedPointer, CopyConstructor)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> std_shared_ptr2 = std_shared_ptr1;

    resource_t resource(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource);
    mito::utilities::SharedPointer<resource_t> shared_ptr2 = shared_ptr1;

    EXPECT_EQ(shared_ptr1.references(), std_shared_ptr1.use_count());
    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, MoveConstructor)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> std_shared_ptr2(std::move(std_shared_ptr1));

    resource_t resource(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource);
    mito::utilities::SharedPointer<resource_t> shared_ptr2(std::move(shared_ptr1));

    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, AssignmentOperator)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    auto std_shared_ptr2 = std::make_shared<int>(1);
    std_shared_ptr2 = std_shared_ptr1;

    resource_t resource1(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
    resource_t resource2(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr2(&resource2);
    shared_ptr2 = shared_ptr1;

    EXPECT_EQ(shared_ptr1.references(), std_shared_ptr1.use_count());
    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, MoveAssignmentOperator)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    auto std_shared_ptr2 = std::make_shared<int>(1);
    std_shared_ptr2 = std::move(std_shared_ptr1);

    resource_t resource1(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
    resource_t resource2(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr2(&resource2);
    shared_ptr2 = std::move(shared_ptr1);

    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, ThreeWayAssignment)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> std_shared_ptr2 = std_shared_ptr1;
    auto std_shared_ptr3 = std::make_shared<int>(1);
    std_shared_ptr2 = std_shared_ptr3;

    resource_t resource1(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
    mito::utilities::SharedPointer<resource_t> shared_ptr2 = shared_ptr1;
    resource_t resource3(1);
    mito::utilities::SharedPointer<resource_t> shared_ptr3(&resource3);
    shared_ptr2 = shared_ptr3;

    EXPECT_EQ(shared_ptr1.references(), std_shared_ptr1.use_count());
    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
    EXPECT_EQ(shared_ptr3.references(), std_shared_ptr3.use_count());
}

TEST(SharedPointer, Destructor)
{
    std::shared_ptr<int> std_shared_ptr2;
    {
        auto std_shared_ptr1 = std::make_shared<int>(1);
        std_shared_ptr2 = std_shared_ptr1;
    }

    mito::utilities::SharedPointer<resource_t> shared_ptr2;
    {
        resource_t resource1(1);
        mito::utilities::SharedPointer<resource_t> shared_ptr1(&resource1);
        shared_ptr2 = shared_ptr1;
    }

    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
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