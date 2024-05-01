// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>
#include <mito/io.h>


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


TEST(Manifolds, HalfSphereCartesian)
{
    // a Cartesian coordinate system in 3D
    auto coord_system = mito::geometry::coordinate_system<3, CARTESIAN>();

    // read the mesh of a sphere
    std::ifstream fileStream("bottom_half_sphere.summit");
    auto mesh = mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, coord_system);

    // the normal field to the submanifold
    constexpr auto normal_field = mito::fields::field([](const coordinates_t & x) -> auto {
        mito::scalar_t phi = std::atan2(x[1], x[0]);
        mito::scalar_t theta = std::atan2(std::hypot(x[1], x[0]), x[2]);
        return std::sin(theta) * std::cos(phi) * mito::e_0<3>
             + std::sin(theta) * std::sin(phi) * mito::e_1<3> + std::cos(theta) * mito::e_2<3>;
    });

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
        [w, normal_field](const coordinates_t & x) -> auto { return w(x)(normal_field(x), _, _); });

    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, wS);

    // compute the area of the manifold
    mito::scalar_t area = manifold.volume();

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the sphere in the mesh)
    EXPECT_NEAR(4.0 * std::numbers::pi / 2.0, area, 0.23);
}


// end of file
