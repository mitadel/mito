// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/quadrature.h>


// strip the namespace
using mito::geometry::CARTESIAN;
using mito::vector_t;
using mito::real;
using mito::quadrature::GAUSS;

// the placeholder for empty slots in contractions
using mito::tensor::_;

// cartesian coordinates in 3D
using coordinates_t = mito::geometry::coordinates_t<3, CARTESIAN>;


// the basis for vector fields
static constexpr auto e_x = mito::fields::uniform_field<coordinates_t>(mito::e_0<3>);
static constexpr auto e_y = mito::fields::uniform_field<coordinates_t>(mito::e_1<3>);
static constexpr auto e_z = mito::fields::uniform_field<coordinates_t>(mito::e_2<3>);


TEST(Quadrature, Square)
{
    /**
     * Mesh with four cells:
        (0,1)           (1,1)
          4               2
          +---------------+
          | .           . |
          |   .       .   |
          |     .   .     |
          |       . 3     |
          |     .   .     |
          |   .       .   |
          | .           . |
          +---------------+
          0               1
        (0,0)           (1,0)
    */

    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, CARTESIAN>();

    // an empty mesh of simplicial topology in 3D
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<3>>();

    // build coordinates
    constexpr auto x_0 = mito::geometry::coordinates({ 0.0, 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::coordinates({ 1.0, 0.0, 1.0 });
    constexpr auto x_2 = mito::geometry::coordinates({ 1.0, 1.0, 1.0 });
    constexpr auto x_3 = mito::geometry::coordinates({ 0.5, 0.5, 0.5 });
    constexpr auto x_4 = mito::geometry::coordinates({ 0.0, 1.0, 0.0 });

    // create nodes
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);
    auto node_2 = mito::geometry::node(coord_system, x_2);
    auto node_3 = mito::geometry::node(coord_system, x_3);
    auto node_4 = mito::geometry::node(coord_system, x_4);

    // insert triangles in mesh
    mesh.insert({ node_0, node_1, node_3 });
    mesh.insert({ node_1, node_2, node_3 });
    mesh.insert({ node_2, node_4, node_3 });
    mesh.insert({ node_4, node_0, node_3 });

    // the normal vector to the square
    constexpr auto cross = pyre::tensor::cross(x_1 - x_0, x_2 - x_0);
    constexpr auto normal_vector = cross / pyre::tensor::norm(cross);

    // the basis one-forms
    constexpr auto dx =
        mito::fields::one_form_field(e_x, mito::fields::identity_tensor_field<coordinates_t, 3>);
    constexpr auto dy =
        mito::fields::one_form_field(e_y, mito::fields::identity_tensor_field<coordinates_t, 3>);
    constexpr auto dz =
        mito::fields::one_form_field(e_z, mito::fields::identity_tensor_field<coordinates_t, 3>);

    // the 3D metric volume element
    constexpr auto w = mito::fields::wedge(dx, dy, dz);

    // the 2D restriction of the 3D metric volume element
    constexpr auto wS = mito::fields::field(
        [w, normal_vector](const coordinates_t & x) -> auto { return w(x)(normal_vector, _, _); });

    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, wS);

    // This instantiates a quad rule on the cells (pairing cell type and degree of exactness)
    auto integrator = mito::quadrature::integrator<GAUSS, 2 /* degree of exactness */>(manifold);

    // a scalar field
    auto f_xy = mito::fields::field([](const coordinates_t & x) -> real { return x[0] * x[1]; });

    // integrate the field
    real result = integrator.integrate(f_xy);
    // the exact solution
    constexpr auto exact = real{ 0.35355339059327384 };

    // report
    std::cout << "Integration of x*y in 3D: Result = " << result
              << ", Error = " << std::fabs(result - exact) << std::endl;

    // check the result
    EXPECT_DOUBLE_EQ(result, exact);
}

// end of file
