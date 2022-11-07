#include <gtest/gtest.h>
#include <iostream>
#include <mito/utilities.h>

class Simplex : public mito::utilities::Shareable {
  public:
    Simplex(int foo) : _foo(foo) {}

  private:
    int _foo;
};

TEST(SegmentedContainer, TestSegmentedContainer)
{
    // instantiate a segmented container
    // TOFIX: rename {vector} to {collection}
    mito::utilities::segmented_t<Simplex, 3> vector;

    // assert that the container is empty and with no capacity
    EXPECT_EQ(vector.capacity(), 0);
    EXPECT_EQ(vector.size(), 0);

    // emplace three simplices in the container
    auto simplex0 = vector.emplace(0);
    auto simplex1 = vector.emplace(1);
    auto simplex2 = vector.emplace(2);

    // assert that the container has 3 elements and its capacity is also 3
    EXPECT_EQ(vector.capacity(), 3);
    EXPECT_EQ(vector.size(), 3);

    // erase one simplex
    vector.erase(simplex1);

    // assert that the container has 2 elements and its capacity is still 3
    EXPECT_EQ(vector.capacity(), 3);
    EXPECT_EQ(vector.size(), 2);

    // emplace one simplex
    auto simplex3 = vector.emplace(3);

    // assert that the container has again 3 elements and its capacity is 3
    EXPECT_EQ(vector.capacity(), 3);
    EXPECT_EQ(vector.size(), 3);

    // emplace another simplex (trigger allocation of new segment)
    auto simplex4 = vector.emplace(4);

    // assert that the container has now 4 elements and its capacity is 6
    // (new memory allocation was in fact triggered)
    EXPECT_EQ(vector.capacity(), 6);
    EXPECT_EQ(vector.size(), 4);

    // erase all the simplices
    vector.erase(simplex0);
    vector.erase(simplex2);
    vector.erase(simplex3);
    vector.erase(simplex4);

    // assert that the container is empty and but has still capacity of 6
    EXPECT_EQ(vector.capacity(), 6);
    EXPECT_EQ(vector.size(), 0);

    // free the memory
    vector.clear();
}