// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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


TEST(Utilities, SegmentedVectorMove)
{
    // segment size
    const auto segmentSize = 3;

    // instantiate a segmented vector of {resource_t} resources
    mito::utilities::segmented_vector_t<resource_t> collection(segmentSize);

    // emplace resources in the container
    collection.emplace(0);
    collection.emplace(1);
    collection.emplace(2);
    collection.emplace(3);

    EXPECT_EQ(collection.size(), 4);
    EXPECT_EQ(collection.capacity(), 6);

    mito::utilities::segmented_vector_t<resource_t> collection2(std::move(collection));

    EXPECT_EQ(collection2.size(), 4);
    EXPECT_EQ(collection2.capacity(), 6);

    EXPECT_EQ(collection.size(), 0);
    EXPECT_EQ(collection.capacity(), 0);
}
