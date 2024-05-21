// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/manifolds.h>
#include <mito/io.h>


// strip the namespace
using mito::geometry::CARTESIAN;
using mito::geometry::SPHERICAL;

// spherical coordinates in 3D
using spherical_coordinates_t = mito::geometry::coordinates_t<3, SPHERICAL>;
// cartesian coordinates in 3D
using cartesian_coordinates_t = mito::geometry::coordinates_t<3, CARTESIAN>;

// the basis for vector fields
static constexpr auto e_r = mito::fields::uniform_field<spherical_coordinates_t>(mito::e_0<3>);


TEST(Manifolds, HalfSphereSpherical)
{
    // the coordinate system
    auto cartesian_coord_system = mito::geometry::coordinate_system<cartesian_coordinates_t>();

    // read the mesh of a sphere
    std::ifstream fileStream("bottom_half_sphere.summit");
    auto mesh =
        mito::io::summit::reader<mito::geometry::triangle_t<3>>(fileStream, cartesian_coord_system);

    // perform change of coordinates from cartesian to spherical
    auto coord_system =
        mito::geometry::coordinate_system<spherical_coordinates_t>(cartesian_coord_system);

    // the normal field to the submanifold
    constexpr auto normal_field = e_r;

    // create a submanifold on {mesh} with the appropriate normal field
    auto manifold = mito::manifolds::submanifold(mesh, coord_system, normal_field);

    // compute the area of the manifold
    mito::scalar_t area = manifold.volume();

    // expect a reasonable match with the exact solution
    // (the error depends on the poor approximation of the sphere in the mesh)
    EXPECT_NEAR(4.0 * std::numbers::pi / 2.0, area, 0.23);
}


// end of file
