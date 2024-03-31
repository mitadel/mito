// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
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
                return mito::vector_t<2>{ x0, x1 };
            }))
        // operator[]
        .def(
            "__getitem__",
            // the implementation
            [](const coordinates_2D_t & self, int i) { return self[i]; })
        // done
        ;


    // the mito scalar field 2D
    using scalar_function_2D_t = std::function<mito::scalar_t(const coordinates_2D_t &)>;
    using scalar_field_2D_t = mito::manifolds::field_t<scalar_function_2D_t>;
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
    using scalar_function_3D_t = std::function<mito::scalar_t(const coordinates_3D_t &)>;
    using scalar_field_3D_t = mito::manifolds::field_t<scalar_function_3D_t>;
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

    // alias for a mesh of triangles embedded in 2D
    using mesh_triangle_2D_t =
        mito::mesh::mesh_t<mito::topology::triangle_t, 2, mito::geometry::CARTESIAN>;

    // the mito Mesh interface
    mito::py::class_<mesh_triangle_2D_t>(m, "SimplicialMesh2D")
        // the default constructor
        .def(mito::py::init([](std::string filename) {
            // an empty topology
            auto & topology = mito::topology::topology();
            // an empty cloud of points in 2D
            auto & point_cloud = mito::geometry::point_cloud<2>();
            // an empty geometry binding {topology} and {point_cloud}
            auto & geometry = mito::geometry::geometry(topology, point_cloud);

            // create an input stream
            auto filestream = std::ifstream(filename);
            // read the mesh
            return mesh_triangle_2D_t(
                mito::io::summit::reader<mito::topology::triangle_t, 2>(filestream, geometry));
        }))
        // accessors
        // the cells; read-only property
        .def_property_readonly("cells", &mesh_triangle_2D_t::cells, "the body cells")
        // done
        ;

    // alias for a manifold of triangles embedded in 2D
    using manifold_triangle_2D_t =
        mito::manifolds::manifold_t<mito::geometry::CARTESIAN, mito::topology::triangle_t, 2>;

    // the mito manifold interface
    mito::py::class_<manifold_triangle_2D_t>(m, "ManifoldTriangle2D")
        // the constructor
        .def(
            // the implementation
            mito::py::init([](const mesh_triangle_2D_t & mesh) {
                // instantiate
                return manifold_triangle_2D_t(mesh);
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
        // QUESTION: should this be called integrateScalarfield?
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