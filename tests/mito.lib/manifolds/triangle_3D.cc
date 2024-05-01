// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


// alias for cartesian coordinates
using mito::geometry::CARTESIAN;
// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<3, CARTESIAN>;

// the basis for vector fields
static constexpr auto e_x = mito::fields::uniform_field<coordinates_t>(mito::e_0<3>);
static constexpr auto e_y = mito::fields::uniform_field<coordinates_t>(mito::e_1<3>);
static constexpr auto e_z = mito::fields::uniform_field<coordinates_t>(mito::e_2<3>);


// the placeholder for empty slots in contractions
using mito::tensor::_;


TEST(Manifolds, Triangle3D)
{
    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, mito::geometry::CARTESIAN>();

    // an empty mesh of triangles
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<3>>();

    // build nodes
    constexpr auto x_0 = mito::geometry::coordinates({ 0.0, 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::coordinates({ 1.0, 0.0, 1.0 });
    constexpr auto x_2 = mito::geometry::coordinates({ 1.0, 1.0, 1.0 });

    // the normal vector to the submanifold
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

    // build nodes of a triangle (counterclockwise order)
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);
    auto node_2 = mito::geometry::node(coord_system, x_2);

    // build triangle with a positive volume (reference triangle)
    auto triangle = mito::geometry::triangle<3>({ node_0, node_1, node_2 });

    // insert triangle in the mesh
    mesh.insert(triangle);

    // compute the area of the manifold
    mito::scalar_t area = manifold.volume();

    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), area);
}


// end of file
