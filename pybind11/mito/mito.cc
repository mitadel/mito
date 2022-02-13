// For simplicity, here are both the c++ function and the binding code.
// In practice, implementation and binding code will generally be located in separate files.

// To compile on MacOS:
// >  c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup `python3 -m pybind11 --includes`
// mito.cpp -o mito`python3-config --extension-suffix`
//
// To compile on Linux:
// > c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` mito.cpp -o
// mito`python3-config --extension-suffix`

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <functional>
#include "../../mito/mito.h"
#include "../../mito/mesh.h"
#include "../../mito/math.h"
#include "../../mito/quadrature/Integrator.h"

namespace py = pybind11;

PYBIND11_MODULE(mito, m)
{
    m.doc() = "pybind11 mito plugin";    // optional module docstring


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
    py::class_<mito::math::scalar_field_t<3>>(m, "ScalarField3D")
        // the constructor
        .def(
            // the implementation
            py::init<const mito::functor<mito::vector_t<3>, mito::scalar_t> &>())
        // operator()
        .def(
            "__call__",
            // the implementation
            [](const mito::math::scalar_field_t<3> & self, const mito::vector_t<3> & x) {
                return self(x);
            })
        // done
        ;


    // the mito scalar Field interface
    py::class_<mito::math::scalar_field_t<2>>(m, "ScalarField2D")
        // TOFIX:
        // the default constructor
        .def(
            // the implementation
            py::init([]() {
                return new mito::math::scalar_field_t<2>(
                        [](const mito::vector_t<2> & x) { return cos(x[0] * x[1]); }
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
            [](const mito::math::scalar_field_t<2> & self, const mito::vector_t<2> & x) {
                return self(x);
            })
        // done
        ;


    // the mito Mesh interface
    py::class_<mito::mesh::Mesh<2>>(m, "Mesh2D")
        // the constructor
        .def(
            // the implementation
            py::init<std::string>())
        // accessors
        // the elements; read-only property
        .def_property_readonly("elements", &mito::mesh::Mesh<2>::elements<2>, "the body elements")
        // the vertex-point map; read-only property
        .def_property_readonly("vertices", &mito::mesh::Mesh<2>::vertices, 
            "the positions of the vertices")
        // done
        ;


    // the mito ElementSet interface
    py::class_<mito::mesh::ElementSet<mito::mesh::triangle_t, 2>>(m, "ElementSetTriangle2D")
        // the constructor
        .def(
            // the implementation
            py::init<const std::vector<mito::mesh::triangle_t *> &, const mito::mesh::VertexSet<2> &>())
        // the constructor
        .def(
            // the implementation
            py::init([](std::string filename) {
                // TOFIX: who is going to delete?
                // read the mesh
                mito::mesh::Mesh<2> * mesh = new mito::mesh::Mesh<2>(filename);
                // instantiate
                return new mito::mesh::ElementSet<mito::mesh::triangle_t, 2>(
                    mesh->elements<2>(), 
                    mesh->vertices());
            }))
        // done
        ;


    // the mito Integrator interface
    py::class_<mito::Integrator<
        mito::GAUSS, 2 /* degree of exactness */, mito::mesh::ElementSet<mito::mesh::triangle_t, 2>>>(
        m, "GaussIntegrator2Triangle2D")
        // the constructor
        .def(
            // the implementation
            py::init<const mito::mesh::ElementSet<mito::mesh::triangle_t, 2> &>())
        // interface
        // QUESTION: should this be called integrateScalarfield?
        // integrate a scalar field
        .def(
            "integrate",
            // the method;
            &mito::Integrator<
                mito::GAUSS, 2 /* degree of exactness */,
                mito::mesh::ElementSet<mito::mesh::triangle_t, 2>>::integrate<mito::real>,
            // the docstring
            "integrate a field")
        // done
        ;
}