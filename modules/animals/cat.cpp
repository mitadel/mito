// declare the module
module;

// get support
// the headers included here do not get exported/imported along with the module, this speeds up
// compilation time in large projects
#include <iostream>

// declare the module and export it
export module Cat;

import Animal;

namespace mito {
    export class Cat : public Animal {

      public:
        Cat() : Animal() {}
        ~Cat() {}

        void hello() const override { std::cout << "Hello from cat..." << std::endl; }
    };
}

// end of file
