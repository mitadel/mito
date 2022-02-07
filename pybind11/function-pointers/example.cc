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
#include "../../mito/mito.h"

namespace py = pybind11;

class Integral {
  public:
    using real = double;
    // using X = double;
    using X = mito::vector_t<3>;
    using function_t = std::function<real(X &, real &)>;

  public:
    Integral(function_t function) : _function(function) {};
    ~Integral() {};
    real evaluate(X & x, real & t) { return _function(x, t); }

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
        // evaluate
        .def(
            "evaluate",
            // the method;
            &Integral::evaluate,
            // the docstring
            "integrate function")
        // done
        ;

    // the mito vector interface
    py::class_<mito::vector_t<3>>(m, "Vector3D")
        // the default constructor
        .def(
            // the implementation
            py::init<>())
        // the brace-enclosed initializer list constructor
        .def(
            // the implementation
            py::init([](std::tuple<mito::real, mito::real, mito::real> data) {
                // unpack
                auto [x0, x1, x2] = data;
                // instantiate
                return new mito::vector_t<3> { x0, x1, x2 };
            }))
        // operator[]
        .def(
            "__getitem__",
            // the implementation
            [](const mito::vector_t<3> & self, int i) { return self[i]; })
        // done
        ;
}