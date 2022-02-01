// For simplicity, here are both the c++ function and the binding code.
// In practice, implementation and binding code will generally be located in separate files.

// To compile on MacOS:
// >  c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup `python3 -m pybind11 --includes`
// example.cpp -o example`python3-config --extension-suffix`
//
// To compile on Linux:
// > c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` example.cpp -o
// example`python3-config --extension-suffix`

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <functional>

namespace py = pybind11;

class Integral {
  public:
    using real = double;
    using function_t = std::function<real(real &)>;

  public:
    Integral(function_t function) : _function(function) {};
    ~Integral() {};
    real evaluate(real & x) { return _function(x); }

  private:
    function_t _function;
};


PYBIND11_MODULE(example, m)
{
    m.doc() = "pybind11 example plugin";    // optional module docstring

    // the Integral interface
    py::class_<Integral>(m, "Integral")
        // the constructor
        .def(
            // the implementation
            py::init<Integral::function_t>())
        // interface
        // Load
        .def(
            "evaluate",
            // the method;
            &Integral::evaluate,
            // the docstring
            "integrate function")
        // done
        ;
}