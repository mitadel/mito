// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/manifolds.h>
#include <mito/io.h>


// the type of coordinates
using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;


TEST(Manifolds, HalfSphereCartesian)
{
    // the coordinate system
    auto coord_system = mito::geometry::coordinate_system<coordinates_t>();

    // read the mesh of a sphere
    std::ifstream fileStream("bottom_half_sphere.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, coord_system);

    // the normal field to the submanifold
    constexpr auto normal_field = mito::fields::field([](const coordinates_t & x) -> auto {
        mito::tensor::scalar_t phi = std::atan2(x[1], x[0]);
        mito::tensor::scalar_t theta = std::atan2(std::hypot(x[1], x[0]), x[2]);
        return std::sin(theta) * std::cos(phi) * mito::tensor::e_0<3>
             + std::sin(theta) * std::sin(phi) * mito::tensor::e_1<3>
             + std::cos(theta) * mito::tensor::e_2<3>;
    });

    // create a submanifold on {mesh} with the appropriate normal field
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, normal_field);

    // compute the area of the manifold
    mito::tensor::scalar_t area = manifold.volume();

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the sphere in the mesh)
    EXPECT_NEAR(4.0 * std::numbers::pi / 2.0, area, 0.23);
}


// end of file
