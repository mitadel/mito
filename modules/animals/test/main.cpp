#include <iostream>
// TOFIX: gcc requires this include while clang does not. It should not be necessary to include this
// header file (compiler bug?)
#include "../../../mito/mito.h"    // REMOVE THIS HEADER

// get support from module
import Cat;

int
main()
{
    // use a function imported from the module
    mito::Cat cat;
    cat.hello();
    cat.aFunction();

    // all done
    return 0;
}

// end of file
