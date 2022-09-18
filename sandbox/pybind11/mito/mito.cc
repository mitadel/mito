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
#include <mito/mito.h>

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
            py::init<const mito::math::functor_t<mito::vector_t<3>, mito::scalar_t> &>())
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
        // the constructor
        .def(
            // the implementation
            py::init<const mito::math::functor_t<mito::vector_t<2>, mito::scalar_t> &>())
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
        // the default constructor
        .def(
            // the implementation
            py::init<>())
        // accessors
        // the elements; read-only property
        .def_property_readonly("elements", &mito::mesh::mesh_t<2>::elements<2>, "the body elements")
        // done
        ;


    // the mito manifold interface
    py::class_<mito::manifolds::manifold_t<mito::topology::triangle_t, 2>>(m, "ManifoldTriangle2D")
        // the constructor
        .def(
            // the implementation
            py::init<const mito::topology::element_vector_t<mito::topology::triangle_t> &>())
        // the constructor
        .def(
            // the implementation
            py::init([](std::string filename) {
                // create an input stream
                auto filestream = std::ifstream(filename);
                // read the mesh
                mito::mesh::mesh_t<2> mesh = mito::mesh::summit<2>(filestream);
                // instantiate
                return new mito::manifolds::manifold_t<mito::topology::triangle_t, 2>(
                    mesh.elements<2>());
            }))
        // done
        ;


    // the mito Integrator interface
    py::class_<mito::quadrature::integrator_t<
        mito::quadrature::GAUSS, 2 /* degree of exactness */,
        mito::manifolds::manifold_t<mito::topology::triangle_t, 2>>>(
        m, "GaussIntegrator2Triangle2D")
        // the constructor
        .def(
            // the implementation
            py::init<const mito::manifolds::manifold_t<mito::topology::triangle_t, 2> &>())
        // interface
        // QUESTION: should this be called integrateScalarfield?
        // integrate a scalar field
        .def(
            "integrate",
            // the method;
            &mito::quadrature::integrator_t<
                mito::quadrature::GAUSS, 2 /* degree of exactness */,
                mito::manifolds::manifold_t<mito::topology::triangle_t, 2>>::integrate<mito::real>,
            // the docstring
            "integrate a field")
        // done
        ;
}