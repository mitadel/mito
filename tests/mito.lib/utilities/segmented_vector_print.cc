#include <gtest/gtest.h>
#include <mito/base.h>
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
    // print each entry of the vector
    for (int i = 0; i < std::size(vector); ++i) {
        std::cout << vector[i].foo() << std::endl;
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
    collection.emplace_back(0);
    collection.emplace_back(1);
    collection.emplace_back(2);
    collection.emplace_back(3);
    collection.emplace_back(4);
    collection.emplace_back(5);

    // show me
    print(collection);
}
