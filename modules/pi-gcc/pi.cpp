// global module fragment
module;

/*
 * get support from headers of libraries not modularized so far
 * NOTE: the headers included here do not get exported/imported along with the module, this speeds
 * up compilation time in large projects
 */
#include <iostream>

// this preprocessor variable lives locally in this file
#define CRUNCH_FACTOR 3.1415

// declare the module
module Pi;

/*
 * import of other modules is done here
 */

/*
 * declarations only visible inside the module
 */

// this helper function lives locally in this file because it is not exported
double
applyCrunchFactor(double number)
{
    return number * CRUNCH_FACTOR;
}

// we can have namespaces within modules. The name of this namespace does not need to match the
// module's name
namespace mito {

    // this function is exported, so it is visible outside, when we import this module
    double crunch(double number)
    {
        auto crunched = applyCrunchFactor(number);
        std::cout << "Crunched " << number << " with result of " << crunched << std::endl;
        return crunched;
    }

}    // end of namespace

// end of file
