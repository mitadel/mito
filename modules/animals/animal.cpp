// declare the module
module;

// declare the module and export it
export module Animal;

// we can have namespaces within modules
namespace mito {

    // this class is exported, so it is visible outside, when we import this module
    export class Animal {

      public:
        Animal() {}
        ~Animal() {}

        virtual void hello() const = 0;
    };

}    // end of namespace

// end of file
