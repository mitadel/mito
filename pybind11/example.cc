// For simplicity, here are both the c++ function and the binding code.
// In practice, implementation and binding code will generally be located in separate files.

// To compile on MacOS:
// >  c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup `python3 -m pybind11 --includes` example.cpp -o example`python3-config --extension-suffix`
//
// To compile on Linux:
// > c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` example.cpp -o example`python3-config --extension-suffix`

#include <pybind11/pybind11.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

// PYBIND11_MODULE() is a macro that creates a function that will be called
// when an "import" statement is issued from within Python.
// 2 arguments: 1) The module name ("example")
//              2) The second argument ("m") defines a variable of type "py::module"
//                 which is the main interface for creating bindings.
// The method "module::def()" generates binding code that exposes the "add()" function
// to Python.
PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    // regular notation
    m.def("add1", &add, "A function which adds two numbers",
          py::arg("i") = 1, py::arg("j") = 2);
    // shorthand
    using namespace pybind11::literals;
    m.def("add2", &add, "Same method as add1, but shortand",
          "i"_a = 1, "j"_a = 2);

    // Exporting variables
    m.attr("the_answer") = 42;
    py::object world = py::cast("World");
    m.attr("what") = world;
}