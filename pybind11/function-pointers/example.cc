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
#include "../../mito/mesh/Simplex.h"
#include "../../mito/mesh/Mesh.h"
#include "../../mito/mesh/ElementSet.h"
#include "../../mito/math/Function.h"
#include "../../mito/math/Field.h"
#include "../../mito/quadrature/Integrator.h"

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


    // the mito vector interface
    py::class_<mito::vector_t<2>>(m, "Vector2D")
        // the default constructor
        .def(
            // the implementation
            py::init<>())
        // the brace-enclosed initializer list constructor
        .def(
            // the implementation
            py::init([](std::tuple<mito::real, mito::real> data) {
                // unpack
                auto [x0, x1] = data;
                // instantiate
                return new mito::vector_t<2> { x0, x1 };
            }))
        // operator[]
        .def(
            "__getitem__",
            // the implementation
            [](const mito::vector_t<2> & self, int i) { return self[i]; })
        // done
        ;


    // the mito scalar Field interface
    py::class_<mito::Field<mito::vector_t<3>, mito::scalar_t>>(m, "ScalarField3D")
        // the constructor
        .def(
            // the implementation
            py::init<const mito::functor<mito::vector_t<3>, mito::scalar_t> &>())
        // operator()
        .def(
            "__call__",
            // the implementation
            [](const mito::Field<mito::vector_t<3>, mito::scalar_t> & self, 
                const mito::vector_t<3> & x) { return self(x); })
        // done
        ;


    // the mito scalar Field interface
    py::class_<mito::Field<mito::vector_t<2>, mito::scalar_t>>(m, "ScalarField2D")
        // TOFIX:
        // the default constructor
        .def(
            // the implementation
            py::init([]() {
                return new mito::Field<mito::vector_t<2>, mito::scalar_t>(
                    mito::Function<mito::vector_t<2>>(
                        [](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); })
                );
            }))
        // the constructor
        .def(
            // the implementation
            py::init<const mito::functor<mito::vector_t<2>, mito::scalar_t> &>())
        // operator()
        .def(
            "__call__",
            // the implementation
            [](const mito::Field<mito::vector_t<2>, mito::scalar_t> & self,
               const mito::vector_t<2> & x) { return self(x); })
        // done
        ;


    // the mito Mesh interface
    py::class_<mito::Mesh<2>>(m, "Mesh2D")
        // the constructor
        .def(
            // the implementation
            py::init<std::string>())
        // accessors
        // the elements; read-only property
        .def_property_readonly("elements", &mito::Mesh<2>::getEntities<2>, "the body elements")
        // the vertex-point map; read-only property
        .def_property_readonly("vertices", &mito::Mesh<2>::getVertexPointMap, 
            "the positions of the vertices")
        // done
        ;


    // the mito ElementSet interface
    py::class_<mito::ElementSet<mito::triangle_t, 2>>(m, "ElementSetTriangle2D")
        // the constructor
        .def(
            // the implementation
            py::init<const std::vector<mito::triangle_t *> &, const mito::VertexPointMap<2> &>())
        // the constructor
        .def(
            // the implementation
            py::init([](std::string filename) {
                // TOFIX: who is going to delete?
                // read the mesh
                mito::Mesh<2> * mesh = new mito::Mesh<2>(filename);
                // instantiate
                return new mito::ElementSet<mito::triangle_t, 2>(
                    mesh->getEntities<2>(), 
                    mesh->getVertexPointMap());
            }))
        // done
        ;


    // the mito Integrator interface
    py::class_<mito::Integrator<
        mito::GAUSS, 2 /* degree of exactness */, mito::ElementSet<mito::triangle_t, 2>>>(
        m, "GaussIntegrator2Triangle2D")
        // the constructor
        .def(
            // the implementation
            py::init<const mito::ElementSet<mito::triangle_t, 2> &>())
        // interface
        // QUESTION: should this be called integrateScalarfield?
        // integrate a scalar field
        .def(
            "integrate",
            // the method;
            &mito::Integrator<
                mito::GAUSS, 2 /* degree of exactness */,
                mito::ElementSet<mito::triangle_t, 2>>::integrate<mito::real>,
            // the docstring
            "integrate a field")
        // done
        ;
}