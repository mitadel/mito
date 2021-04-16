#include <iostream>

// get support from module Pi
import Animal;

void
hello()
{
    std::cout << "Hello from cat..." << std::endl;
}

namespace mito {
    class Cat : public Animal {

      public:
        Cat() : Animal() {}
        ~Cat() {}

        void sayHello() const override { return hello(); }
    };
}

int
main()
{
    // use a function imported from the module
    mito::Cat cat;
    cat.sayHello();

    // all done
    return 0;
}

// end of file
