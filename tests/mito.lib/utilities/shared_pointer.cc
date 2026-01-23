// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <memory>
#include <iostream>
#include <mito/utilities.h>


class Resource : public mito::utilities::Shareable {
  public:
    Resource(int a) : _a(a)
    {
        journal::info_t channel("tests.resource");
        channel << "Built resource " << _a << journal::endl;
    }

    ~Resource()
    {
        journal::info_t channel("tests.~resource");
        channel << "Destroying resource " << _a << journal::endl;
    }

    int _a;
};

// the type of resource to be stored
using resource_t = Resource;
using shared_ptr_t = mito::utilities::shared_ptr<resource_t>;


TEST(SharedPointer, DefaultConstructor)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);

    shared_ptr_t shared_ptr1(new resource_t(1));

    EXPECT_EQ(shared_ptr1.references(), std_shared_ptr1.use_count());
}

TEST(SharedPointer, CopyConstructor)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> std_shared_ptr2 = std_shared_ptr1;

    shared_ptr_t shared_ptr1(new resource_t(1));
    shared_ptr_t shared_ptr2 = shared_ptr1;

    EXPECT_EQ(shared_ptr1.references(), std_shared_ptr1.use_count());
    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, MoveConstructor)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> std_shared_ptr2(std::move(std_shared_ptr1));

    shared_ptr_t shared_ptr1(new resource_t(1));
    shared_ptr_t shared_ptr2(std::move(shared_ptr1));

    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, AssignmentOperator)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    auto std_shared_ptr2 = std::make_shared<int>(1);
    std_shared_ptr2 = std_shared_ptr1;

    shared_ptr_t shared_ptr1(new resource_t(1));
    shared_ptr_t shared_ptr2(new resource_t(1));
    shared_ptr2 = shared_ptr1;

    EXPECT_EQ(shared_ptr1.references(), std_shared_ptr1.use_count());
    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, MoveAssignmentOperator)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    auto std_shared_ptr2 = std::make_shared<int>(1);
    std_shared_ptr2 = std::move(std_shared_ptr1);

    shared_ptr_t shared_ptr1(new resource_t(1));
    shared_ptr_t shared_ptr2(new resource_t(1));
    shared_ptr2 = std::move(shared_ptr1);

    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

TEST(SharedPointer, ThreeWayAssignment)
{
    auto std_shared_ptr1 = std::make_shared<int>(1);
    std::shared_ptr<int> std_shared_ptr2 = std_shared_ptr1;
    auto std_shared_ptr3 = std::make_shared<int>(1);
    std_shared_ptr2 = std_shared_ptr3;

    shared_ptr_t shared_ptr1(new resource_t(1));
    shared_ptr_t shared_ptr2 = shared_ptr1;
    shared_ptr_t shared_ptr3(new resource_t(1));
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

    shared_ptr_t shared_ptr2;
    {
        shared_ptr_t shared_ptr1(new resource_t(1));
        shared_ptr2 = shared_ptr1;
    }

    EXPECT_EQ(shared_ptr2.references(), std_shared_ptr2.use_count());
}

template <class RESOURCE>
class Segment {

  public:
    using resource_t = RESOURCE;
    using resource_ptr = RESOURCE *;

  public:
    static auto create(int N)
    {
        resource_ptr segment = static_cast<resource_ptr>(::operator new(N * sizeof(resource_t)));
        return segment;
    }

    ~Segment()
    {
        // free the segment of memory
        ::operator delete(segment);
    }

  private:
    static resource_ptr segment;
};

TEST(SharedPointer, TestSegmentAllocation)
{
    // the size of the segment of memory
    constexpr int N = 10;
    // the initialization value for the resource
    int a = 10;

    // allocate a new segment of memory
    resource_t * segment = Segment<resource_t>::create(N);

    // select a location within the segment of memory
    resource_t * location = (segment + 3);

    // instantiate new resource at {location}
    resource_t * resource = new (location) resource_t(a);
    mito::utilities::shared_ptr<resource_t> handle(resource);

    // modify the resource
    handle->_a += 1;

    // assert that the resource was modified correctly
    EXPECT_EQ(handle->_a, a + 1);
}