// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/utilities.h>


class Resource : public mito::utilities::Invalidatable {
  public:
    Resource(int foo) : _foo(foo) {}

  private:
    int _foo;
};

// the resource type
using resource_t = Resource;


TEST(Utilities, SegmentedVectorErase)
{
    // segment size
    const auto segmentSize = 3;

    // instantiate a segmented vector of {resource_t} resources
    mito::utilities::segmented_vector_t<resource_t> collection(segmentSize);

    // assert that the container is empty and with no capacity
    EXPECT_EQ(collection.capacity(), 0);
    EXPECT_EQ(std::size(collection), 0);

    // emplace two resources in the container
    auto & resource_0 = collection.emplace(0);
    auto & resource_1 = collection.emplace(1);

    // assert that the container has 2 elements and its capacity is 3
    EXPECT_EQ(collection.capacity(), 3);
    EXPECT_EQ(std::size(collection), 2);

    // erase the resources from the container
    collection.erase(resource_0);
    collection.erase(resource_1);

    // assert that the container has 0 elements and its capacity is 3
    EXPECT_EQ(collection.capacity(), 3);
    EXPECT_EQ(std::size(collection), 0);

    // emplace two new resources in the container
    collection.emplace(2);
    collection.emplace(3);

    // assert that the container has 2 elements and its capacity is 3
    EXPECT_EQ(collection.capacity(), 3);
    EXPECT_EQ(std::size(collection), 2);
}
