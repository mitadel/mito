// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

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

    // the mito cartesian coordinates interface
    using coordinates_3D_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;
    mito::py::class_<coordinates_3D_t>(m, "Coordinates3D")
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
                return coordinates_3D_t{ x0, x1, x2 };
            }))
        // operator[]
        .def(
            "__getitem__",
            // the implementation
            [](const coordinates_3D_t & self, int i) { return self[i]; })
        // done
        ;


    // the mito vector interface
    using coordinates_2D_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;
    mito::py::class_<coordinates_2D_t>(m, "Coordinates2D")
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
                return mito::tensor::vector_t<2>{ x0, x1 };
            }))
        // operator[]
        .def(
            "__getitem__",
            // the implementation
            [](const coordinates_2D_t & self, int i) { return self[i]; })
        // done
        ;


    // the mito scalar field 2D
    using scalar_function_2D_t = std::function<mito::tensor::scalar_t(const coordinates_2D_t &)>;
    using scalar_field_2D_t = mito::functions::FunctionFromFunctor<scalar_function_2D_t>;
    mito::py::class_<scalar_field_2D_t>(m, "ScalarField2D")
        // the constructor
        .def(mito::py::init<scalar_function_2D_t>())
        // operator()
        .def(
            "__call__",
            // the implementation
            [](const scalar_field_2D_t & self, const coordinates_2D_t & x) { return self(x); })
        // done
        ;


    // the mito scalar field 3D
    using scalar_function_3D_t = std::function<mito::tensor::scalar_t(const coordinates_3D_t &)>;
    using scalar_field_3D_t = mito::functions::FunctionFromFunctor<scalar_function_3D_t>;
    mito::py::class_<scalar_field_3D_t>(m, "ScalarField3D")
        // the constructor
        .def(mito::py::init<scalar_function_3D_t>())
        // operator()
        .def(
            "__call__",
            // the implementation
            [](const scalar_field_3D_t & self, const coordinates_3D_t & x) { return self(x); })
        // done
        ;


    // alias for a coordinate system in 2D
    using coordinate_system_2D_t = mito::geometry::coordinate_system_t<coordinates_2D_t>;
    // the mito coordinate system 2D
    mito::py::class_<coordinate_system_2D_t>(m, "CoordinateSystem2D")
        // the default constructor
        .def(mito::py::init<>())
        // done
        ;


    // alias for a triangle embedded in 2D
    using cell_2D_t = mito::geometry::triangle_t<2>;
    // alias for a mesh of triangles embedded in 2D
    using mesh_triangle_2D_t = mito::mesh::mesh_t<cell_2D_t>;
    // the mito Mesh interface
    mito::py::class_<mesh_triangle_2D_t>(m, "SimplicialMesh2D")
        // the default constructor
        .def(mito::py::init([](std::string filename, coordinate_system_2D_t & coord_system) {
            // create an input stream
            auto fileStream = std::ifstream(filename);
            // read the mesh
            return mesh_triangle_2D_t(
                mito::io::summit::reader<cell_2D_t>(fileStream, coord_system));
        }))
        // accessors
        // the cells; read-only property
        .def_property_readonly(
            "cells",
            static_cast<const mesh_triangle_2D_t::cells_type & (mesh_triangle_2D_t::*) () const>(
                &mesh_triangle_2D_t::cells),
            "the body cells")
        // done
        ;


    // alias for a manifold of triangles embedded in 2D
    using manifold_triangle_2D_t = decltype(mito::manifolds::manifold(
        std::declval<mesh_triangle_2D_t>(), std::declval<coordinate_system_2D_t>()));
    // the mito manifold interface
    mito::py::class_<manifold_triangle_2D_t>(m, "ManifoldTriangle2D")
        // the constructor
        .def(
            // the implementation
            mito::py::init(
                [](const mesh_triangle_2D_t & mesh, const coordinate_system_2D_t & coord_system) {
                    // create the manifold
                    return mito::manifolds::manifold(mesh, coord_system);
                }))
        // done
        ;


    // the mito Integrator interface
    using gauss_integrator_2_triangle_2D_t = mito::quadrature::integrator_t<
        mito::quadrature::GAUSS, 2 /* degree of exactness */, manifold_triangle_2D_t>;
    mito::py::class_<gauss_integrator_2_triangle_2D_t>(m, "GaussIntegrator2Triangle2D")
        // the constructor
        .def(
            // the implementation
            mito::py::init<const manifold_triangle_2D_t &>())
        // interface
        // integrate a scalar field
        .def(
            "integrate",
            // the implementation
            [](const gauss_integrator_2_triangle_2D_t & self, const scalar_field_2D_t & f) {
                return self.integrate(f);
            })
        // done
        ;
}