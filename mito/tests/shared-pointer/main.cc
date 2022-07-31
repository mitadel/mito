#include "../../base.h"
#include "../../utilities/SharedPointer.h"

class Resource {
  public:
    Resource(int a) : _a(a) { std::cout << "Built resource " << _a << std::endl; }

    ~Resource() { std::cout << "Destroying resource " << _a << std::endl; }

  private:
    int _a;
};

int
main()
{

    // allocate a new segment of memory
    Resource * segment = static_cast<Resource *>(::operator new(10 * sizeof(Resource)));

    Resource * location = (segment + 1);
    mito::utilities::SharedPointer<Resource, true> handle(new (location) Resource(1));
    // mito::utilities::SharedPointer<Resource, true> handle(10, location);

    ::operator delete(segment);

    // all done
    return 0;
}