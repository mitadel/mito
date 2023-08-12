#include "Singleton.h"

// a sample singleton class
class SingletonClass {

private:
    // default constructor
    inline SingletonClass() {}

    // copy constructor
    inline SingletonClass(const SingletonClass &) = delete;

    // move constructor
    inline SingletonClass(SingletonClass &&) = delete;

    // assignment operator
    inline SingletonClass & operator=(const SingletonClass &) = delete;

    // move operator=
    inline SingletonClass & operator=(SingletonClass &&) = delete;

    // destructor
    inline ~SingletonClass() {}

public:
    // do something
    inline void foo() const {}

private:
    // friendship with the singleton
    friend mito::utilities::Singleton<SingletonClass>;
};


// typedef for the singleton class
using singleton_t = SingletonClass;


// the singleton factory
auto singleton_instance() -> singleton_t &
{
    return mito::utilities::Singleton<singleton_t>::GetInstance();
}


int main() {

    // create an instance of the singleton class
    singleton_t & singleton = singleton_instance();

    // do something
    singleton.foo();

    // all done
    return 0;
}