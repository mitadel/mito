// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

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


TEST(Utilities, SegmentedVector)
{
    // segment size
    const auto segmentSize = 3;

    // instantiate a segmented vector of {resource_t} resources
    mito::utilities::segmented_vector_t<resource_t> collection(segmentSize);

    // assert that the container is empty and with no capacity
    EXPECT_EQ(collection.capacity(), 0);
    EXPECT_EQ(std::size(collection), 0);

    // emplace two resources in the container
    collection.emplace_back(0);
    collection.emplace_back(1);

    // assert that the container has 2 elements and its capacity is 3
    EXPECT_EQ(collection.capacity(), 3);
    EXPECT_EQ(std::size(collection), 2);

    // emplace one more resource
    auto & resource_2 = collection.emplace_back(2);

    // assert that the container has 3 elements and its capacity is 3
    EXPECT_EQ(collection.capacity(), 3);
    EXPECT_EQ(std::size(collection), 3);

    // erase it
    collection.erase(resource_2);

    // assert that the container has 2 elements and its capacity is still 3
    EXPECT_EQ(collection.capacity(), 3);
    EXPECT_EQ(std::size(collection), 2);

    // emplace one resource
    auto & resource_3 = collection.emplace_back(3);

    // assert that the container has again 3 elements and its capacity is 3
    EXPECT_EQ(collection.capacity(), 3);
    EXPECT_EQ(std::size(collection), 3);

    // emplace another resource (trigger allocation of new segment)
    auto & resource_4 = collection.emplace_back(4);

    // assert that the container has now 4 elements and its capacity is 6
    // (new memory allocation was in fact triggered)
    EXPECT_EQ(collection.capacity(), 6);
    EXPECT_EQ(std::size(collection), 4);

    // erase more resources
    collection.erase(resource_3);
    collection.erase(resource_4);

    // assert that the container has now 2 elements but has still capacity of 6
    EXPECT_EQ(collection.capacity(), 6);
    EXPECT_EQ(std::size(collection), 2);
}
