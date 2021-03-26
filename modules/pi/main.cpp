// Playing with C++20 modules
// Following https://pspdfkit.com/blog/2020/cpp20-in-2020-modules/
// and https://quuxplusone.github.io/blog/2019/11/07/modular-hello-world/
// Check also https://clang.llvm.org/docs/Modules.html on how to build with modules with clang
// Check also https://accu.org/journals/overload/28/159/sidwell/ on how to build modules with gcc
//
// To compile this file do the following
// Build module first: 
//  clang++ -std=c++2a -c pi.cpp -Xclang -emit-module-interface -o pi.pcm
// Then build main file
//  clang++ -std=c++2a -fprebuilt-module-path=. main.cpp pi.cpp

#include <iostream>

// get support from module Pi
import Pi;

int main() {
    // use a function imported from the module
    auto value = numbers::crunch(42);

    // say something
    std::cout << "value = " << value << std::endl;

    #ifndef CRUNCH_FACTOR
        std::cout << "Note that CRUNCH_FACTOR is not defined here, although it is defined in the " 
            << "module" << std::endl;
    #endif

    // all done
    return 0;
}

// end of file
