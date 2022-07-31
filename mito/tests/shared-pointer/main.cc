#include "../../base.h"
#include "../../utilities/SharedPointer.h"

class Resource {
  public:
    Resource(int a, int b, Resource * c) : _a(a)
    {
        std::cout << "Built resource " << _a << " " << b << " " << c << std::endl;
    }

    ~Resource() { std::cout << "Destroying resource " << _a << std::endl; }

  private:
    int _a;
};

int
main()
{

    using resource_t = Resource;

    // allocate a new segment of memory
    resource_t * segment = static_cast<resource_t *>(::operator new(10 * sizeof(resource_t)));

    resource_t * location = (segment + 1);
    mito::utilities::SharedPointer<Resource, true> handle(10, 12, nullptr, location);

    ::operator delete(segment);

    // all done
    return 0;
}