#include "../../base.h"
#include "../../utilities/SharedPointer.h"

class Resource {
  public:
    Resource(int a, Resource * c) : _a(a)
    {
        std::cout << "Built resource " << _a << " " << c << std::endl;
    }

    ~Resource() { std::cout << "Destroying resource " << _a << std::endl; }

    int _a;
};

int
main()
{
    // the type of resource to be stored
    using resource_t = Resource;
    // the size of the segment of memory
    constexpr int N = 10;
    // the initialization value for the resource
    int a = 10;

    // allocate a new segment of memory
    resource_t * segment = static_cast<resource_t *>(::operator new(N * sizeof(resource_t)));

    // select a location within the segment of memory
    resource_t * location = (segment + 3);
    // instantiate new resource at {location}
    mito::utilities::SharedPointer<resource_t, true> handle(a, nullptr, location);

    // assert that the resource is accessible correctly
    assert(location->_a == a);

    // free the segment of memory
    ::operator delete(segment);

    // all done
    return 0;
}