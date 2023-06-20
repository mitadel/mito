// For simplicity, here are both the c++ function and the binding code.
// In practice, implementation and binding code will generally be located in separate files.

// To compile on MacOS:
// >  c++ -O3 -Wall -shared -std=c++11 -undefined dynamic_lookup `python3 -m pybind11 --includes`
// mito.cpp -o mito`python3-config --extension-suffix`
//
// To compile on Linux:
// > c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` mito.cpp -o
// mito`python3-config --extension-suffix`

// external dependencies
#include "external.h"

PYBIND11_MODULE(mito, m)
{
    m.doc() = "pybind11 mito plugin";    // optional module docstring

    // the mito vector interface
    mito::py::class_<mito::vector_t<3>>(m, "Vector3D")
        // the default constructor
        .def(
            // the implementation
            mito::py::init<>())
        // the brace-enclosed initializer list constructor
        .def(
            // the implementation
            mito::py::init([](std::tuple<mito::real, mito::real, mito::real> data) {
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
    mito::py::class_<mito::vector_t<2>>(m, "Vector2D")
        // the default constructor
        .def(
            // the implementation
            mito::py::init<>())
        // the brace-enclosed initializer list constructor
        .def(
            // the implementation
            mito::py::init([](std::tuple<mito::real, mito::real> data) {
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
    mito::py::class_<mito::math::scalar_field_t<3>>(m, "ScalarField3D")
        // the constructor
        .def(
            // the implementation
            mito::py::init<const mito::math::functor_t<mito::vector_t<3>, mito::scalar_t> &>())
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
    mito::py::class_<mito::math::scalar_field_t<2>>(m, "ScalarField2D")
        // the constructor
        .def(
            // the implementation
            mito::py::init<const mito::math::functor_t<mito::vector_t<2>, mito::scalar_t> &>())
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
    mito::py::class_<mito::mesh::Mesh<mito::topology::triangle_t, 2>>(m, "SimplicialMesh2D")
        // the default constructor
        .def(
            // the implementation
            mito::py::init<mito::geometry::Geometry<2> &>())
        // accessors
        // the cells; read-only property
        .def_property_readonly(
            "cells", &mito::mesh::mesh_t<mito::topology::triangle_t, 2>::cells, "the body cells")
        // done
        ;


    // the mito manifold interface
    mito::py::class_<mito::manifolds::manifold_t<mito::topology::triangle_t, 2>>(
        m, "ManifoldTriangle2D")
        // the constructor
        .def(
            // the implementation
            mito::py::init([](std::string filename) {
                // TOFIX: who is going to delete?
                // an empty topology
                auto topology = new mito::topology::topology_t();
                // an empty cloud of points in 2D
                auto point_cloud = new mito::geometry::point_cloud_t<2>();
                // an empty geometry binding {topology} and {point_cloud}
                auto geometry = new mito::geometry::geometry_t<2>(*topology, *point_cloud);

                // create an input stream
                auto filestream = std::ifstream(filename);
                // read the mesh
                auto mesh = new mito::mesh::mesh_t<mito::topology::triangle_t, 2>(
                    mito::io::summit::reader<mito::topology::triangle_t, 2>(filestream, *geometry));
                // instantiate
                return new mito::manifolds::manifold_t<mito::topology::triangle_t, 2>(*mesh);
            }))
        // done
        ;


    // the mito Integrator interface
    mito::py::class_<mito::quadrature::integrator_t<
        mito::quadrature::GAUSS, 2 /* degree of exactness */,
        mito::manifolds::manifold_t<mito::topology::triangle_t, 2>>>(
        m, "GaussIntegrator2Triangle2D")
        // the constructor
        .def(
            // the implementation
            mito::py::init<const mito::manifolds::manifold_t<mito::topology::triangle_t, 2> &>())
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