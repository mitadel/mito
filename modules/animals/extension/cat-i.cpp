// declare the module
module;

// declare the module and export it
export module Cat;

import Animal;

namespace mito {
    export class Cat : public Animal {

      public:
        Cat() : Animal() {}
        ~Cat() {}

        void hello() const override;
    };
}

// end of file
