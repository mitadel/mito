// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>
#include <mito/io.h>


// strip the namespace
using mito::geometry::CARTESIAN;
using mito::geometry::SPHERICAL;
// the placeholder for empty slots in contractions
using mito::tensor::_;

// spherical coordinates in 3D
using spherical_coordinates_t = mito::geometry::coordinates_t<3, SPHERICAL>;
// cartesian coordinates in 3D
using cartesian_coordinates_t = mito::geometry::coordinates_t<3, CARTESIAN>;


// the function extracting the x_0 (i.e. the radial) component of a 3D vector
static constexpr auto r = mito::functions::component<spherical_coordinates_t, 0>;
// the function extracting the x_1 (i.e. the theta) component of a 3D vector
static constexpr auto t = mito::functions::component<spherical_coordinates_t, 1>;

// the basis for vector fields
static constexpr auto e_r = mito::fields::uniform_field<spherical_coordinates_t>(mito::e_0<3>);
static constexpr auto e_t = mito::fields::uniform_field<spherical_coordinates_t>(mito::e_1<3>);
static constexpr auto e_p = mito::fields::uniform_field<spherical_coordinates_t>(mito::e_2<3>);

// the basis for diagonal second-order tensor fields (e_rr, e_thetatheta, e_phiphi)
static constexpr auto e_rr = mito::fields::uniform_field<spherical_coordinates_t>(mito::e_00<3>);
static constexpr auto e_tt = mito::fields::uniform_field<spherical_coordinates_t>(mito::e_11<3>);
static constexpr auto e_pp = mito::fields::uniform_field<spherical_coordinates_t>(mito::e_22<3>);


TEST(Manifolds, HalfSphereSpherical)
{
    // a Cartesian coordinate system in 3D
    auto cartesian_coord_system = mito::geometry::coordinate_system<3, CARTESIAN>();

    // read the mesh of a sphere
    std::ifstream fileStream("bottom_half_sphere.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, cartesian_coord_system);

    // perform change of coordinates from {CARTESIAN} to {SPHERICAL}
    auto coord_system = mito::geometry::coordinate_system<SPHERICAL>(cartesian_coord_system);

    // the metric field
    // e_rr + r^2 * e_tt + r^2 sin^2(theta) * e_pp
    constexpr auto g =
        e_rr + r * r * e_tt + r * mito::functions::sin(t) * r * mito::functions::sin(t) * e_pp;

    // the inverse metric field
    // e_rr + 1/r^2 * e_tt + 1/(r^2 sin^2(theta)) * e_pp
    constexpr auto g_inv = e_rr + 1.0 / (r * r) * e_tt
                         + 1.0 / (r * mito::functions::sin(t) * r * mito::functions::sin(t)) * e_pp;

    // the normal field to the submanifold
    constexpr auto normal_field =
        mito::fields::field([](const spherical_coordinates_t & x) -> auto { return e_r(x); });

    // the basis one-forms
    constexpr auto dr = mito::fields::one_form_field(g_inv * e_r, g);
    constexpr auto dt = mito::fields::one_form_field(g_inv * e_t, g);
    constexpr auto dp = mito::fields::one_form_field(g_inv * e_p, g);

    // the 3D metric volume element
    constexpr auto w =
        mito::fields::sqrt(mito::fields::determinant(g)) * mito::fields::wedge(dr, dt, dp);

    // the 2D restriction of the 3D metric volume element
    constexpr auto wS =
        mito::fields::field([w, normal_field](const spherical_coordinates_t & x) -> auto {
            return w(x)(normal_field(x), _, _);
        });

    // create a submanifold on {mesh} with the appropriate metric volume element {wS}
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, wS);

    // compute the area of the manifold
    mito::scalar_t area = manifold.volume();

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the sphere in the mesh)
    EXPECT_NEAR(4.0 * std::numbers::pi / 2.0, area, 0.23);
}


// end of file
