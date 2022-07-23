#include "../../base.h"
#include "../../utilities/Handle.h"
#include "../../utilities/Shareable.h"

class Resource {
  public:

    using handle_t = Resource *;
    using shared_t = pyre::mpi::Shareable<Resource>;

    Resource(int a): _a(a) {
        std::cout << "Built resource " << _a << std::endl;   
    }

    ~Resource() {
        std::cout << "Destroying resource " << _a << std::endl;
    }

    static inline void free(handle_t *) {
        return;
    }

  private:
    int _a;
};

int main() {

    Resource resource(1);

    pyre::mpi::Handle<Resource> handle(&resource, false);

    // all done 
    return 0;
}