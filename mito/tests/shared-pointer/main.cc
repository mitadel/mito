#include "../../base.h"
#include "../../utilities/SharedPointer.h"
#include "../../utilities/Shareable.h"

class Resource {
  public:
    using handle_t = Resource *;
    using shared_t = mito::utilities::Shareable<Resource, true /* TOFIX */>;

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
    mito::utilities::Handle<Resource, true> handle(new (location) Resource(1));

    ::operator delete(segment);

    // all done
    return 0;
}