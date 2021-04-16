// declare the module
module;

// get support
// the headers included here do not get exported/imported along with the module, this speeds up
// compilation time in large projects
#include <iostream>

// declare the module and export it
module Cat;

void
mito::Cat::hello() const
{
    std::cout << "Hello from cat..." << std::endl;
}

// end of file
