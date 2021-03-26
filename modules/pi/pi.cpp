// declare the module
module;

// get support
// the headers included here do not get exported/imported along with the module, this speeds up 
// compilation time in large projects 
#include <iostream>

// this preprocessor variable lives locally in this file 
#define CRUNCH_FACTOR 3.1415

// declare the module and export it
export module Pi;

// we can have namespaces within modules
namespace numbers {

// this helper function lives locally in this file because it is not exported 
double applyCrunchFactor(double number) {
    return number * CRUNCH_FACTOR;
}

// this function is exported, so it is visible outside, when we import this module 
export double crunch(double number) {
    auto crunched = applyCrunchFactor(number);
    std::cout << "Crunched " << number << " with result of " << crunched << std::endl;
    return crunched;
}

} // end of namespace

// end of file
