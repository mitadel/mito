#include <gtest/gtest.h>
#include <mito/utilities.h>


class Resource : public mito::utilities::Invalidatable {
  public:
    Resource(int foo) : _foo(foo) {}

    int foo() const { return _foo; }

  private:
    int _foo;
};


// the resource type
using resource_t = Resource;
using segmented_vector_t = mito::utilities::segmented_vector_t<resource_t>;


void
print(const segmented_vector_t & vector)
{
    // make a channel
    journal::info_t channel("tests.utilities");

    // print each entry of the vector
    for (int i = 0; i < std::size(vector); ++i) {
        channel << vector[i].foo() << journal::endl;
    }

    // all done
    return;
}


TEST(Utilities, SegmentedVectorSubscript)
{
    // segment size
    const auto segmentSize = 3;

    // instantiate a segmented vector of {resource_t} resources
    segmented_vector_t collection(segmentSize);

    // fill in the container
    collection.emplace(0);
    collection.emplace(1);
    collection.emplace(2);
    collection.emplace(3);
    collection.emplace(4);
    collection.emplace(5);

    // show me
    print(collection);
}
