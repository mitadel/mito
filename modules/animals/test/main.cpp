#include <iostream>

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
