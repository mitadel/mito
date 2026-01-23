// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/manifolds.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Manifolds, Triangle3D)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // an empty mesh of triangles
    auto mesh = mito::mesh::mesh<mito::geometry::triangle_t<3>>();

    // build nodes
    constexpr auto x_0 = mito::geometry::cartesian::coordinates({ 0.0, 0.0, 0.0 });
    constexpr auto x_1 = mito::geometry::cartesian::coordinates({ 1.0, 0.0, 1.0 });
    constexpr auto x_2 = mito::geometry::cartesian::coordinates({ 1.0, 1.0, 1.0 });

    // the normal vector to the submanifold
    constexpr auto cross = mito::tensor::cross(x_1 - x_0, x_2 - x_0);
    constexpr auto normal_vector = cross / mito::tensor::norm(cross);
    constexpr auto normal_field = mito::functions::constant<coordinates_t>(normal_vector);

    // create a submanifold on {mesh} with the appropriate normal field
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, normal_field);

    // build nodes of a triangle (counterclockwise order)
    auto node_0 = mito::geometry::node(coord_system, x_0);
    auto node_1 = mito::geometry::node(coord_system, x_1);
    auto node_2 = mito::geometry::node(coord_system, x_2);

    // build triangle with a positive volume (reference triangle)
    auto triangle = mito::geometry::triangle<3>({ node_0, node_1, node_2 });

    // insert triangle in the mesh
    mesh.insert(triangle);

    // compute the area of the manifold
    mito::tensor::scalar_t area = manifold.volume();

    // check that the volume of triangle is correct
    EXPECT_DOUBLE_EQ(0.5 * std::sqrt(2.0), area);
}


// end of file
