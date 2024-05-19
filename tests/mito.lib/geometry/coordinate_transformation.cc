// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


TEST(CoordinateTransformation, CartesianPolar)
{
    // cartesian coordinates in 2D
    using cartesian_coord_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

    // polar coordinates in 2D
    using polar_coord_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;

    // create a set of coordinates
    auto coord_cart_A = mito::geometry::coordinates<cartesian_coord_t>({ 1.0, 1.0 });

    // transform it to polar coordinates
    auto coord_polar_A = transform_coordinates<polar_coord_t>(coord_cart_A);

    // transform it back to cartesian coordinates
    auto coord_cart_back_A = transform_coordinates<cartesian_coord_t>(coord_polar_A);

    // check that we obtained the initial set of coordinates
    EXPECT_DOUBLE_EQ(coord_cart_A[0], coord_cart_back_A[0]);
    EXPECT_DOUBLE_EQ(coord_cart_A[1], coord_cart_back_A[1]);

    // create another set of coordinates
    auto coord_cart_B = mito::geometry::coordinates<cartesian_coord_t>({ 1.0, 0.0 });

    // transform it to polar coordinates
    auto coord_polar_B = transform_coordinates<polar_coord_t>(coord_cart_B);

    // midpoint of A and B in cartesian coordinates
    auto coord_cart_AB = coord_cart_A + 0.5 * (coord_cart_B - coord_cart_A);

    // midpoint of A and B in polar coordinates
    auto coord_polar_AB = coord_polar_A + 0.5 * (coord_polar_B - coord_polar_A);

    // transform the cartesian sum to polar coordinates
    auto coord_cart_AB_polar = transform_coordinates<polar_coord_t>(coord_cart_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_polar_AB[0], coord_cart_AB_polar[0]);
    EXPECT_DOUBLE_EQ(coord_polar_AB[1], coord_cart_AB_polar[1]);

    // transform the polar sum to cartesian coordinates
    auto coord_polar_AB_cart = transform_coordinates<cartesian_coord_t>(coord_polar_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_cart_AB[0], coord_polar_AB_cart[0]);
    EXPECT_DOUBLE_EQ(coord_cart_AB[1], coord_polar_AB_cart[1]);
}


TEST(CoordinateTransformation, CartesianSpherical)
{
    // cartesian coordinates in 3D
    using cartesian_coord_t = mito::geometry::coordinates_t<3, mito::geometry::CARTESIAN>;

    // spherical coordinates in 3D
    using spherical_coord_t = mito::geometry::coordinates_t<3, mito::geometry::SPHERICAL>;

    // create a set of coordinates
    auto coord_cart_A = mito::geometry::coordinates<cartesian_coord_t>({ 1.0, 1.0, 1.0 });

    // transform it to polar coordinates
    auto coord_spherical_A = transform_coordinates<spherical_coord_t>(coord_cart_A);

    // transform it back to cartesian coordinates
    auto coord_cart_back_A = transform_coordinates<cartesian_coord_t>(coord_spherical_A);

    // check that we obtained the initial set of coordinates
    EXPECT_DOUBLE_EQ(coord_cart_A[0], coord_cart_back_A[0]);
    EXPECT_DOUBLE_EQ(coord_cart_A[1], coord_cart_back_A[1]);
    EXPECT_DOUBLE_EQ(coord_cart_A[2], coord_cart_back_A[2]);

    // create another set of coordinates
    auto coord_cart_B = mito::geometry::coordinates<cartesian_coord_t>({ 0.5, 0.0, 1.0 });

    // transform it to polar coordinates
    auto coord_spherical_B = transform_coordinates<spherical_coord_t>(coord_cart_B);

    // midpoint of A and B in cartesian coordinates
    auto coord_cart_AB = coord_cart_A + 0.5 * (coord_cart_B - coord_cart_A);

    // midpoint of A and B in polar coordinates
    auto coord_spherical_AB = coord_spherical_A + 0.5 * (coord_spherical_B - coord_spherical_A);

    // transform the cartesian sum to polar coordinates
    auto coord_cart_AB_spherical = transform_coordinates<spherical_coord_t>(coord_cart_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_spherical_AB[0], coord_cart_AB_spherical[0]);
    EXPECT_DOUBLE_EQ(coord_spherical_AB[1], coord_cart_AB_spherical[1]);
    EXPECT_DOUBLE_EQ(coord_spherical_AB[2], coord_cart_AB_spherical[2]);

    // transform the polar sum to cartesian coordinates
    auto coord_spherical_AB_cart = transform_coordinates<cartesian_coord_t>(coord_spherical_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_cart_AB[0], coord_spherical_AB_cart[0]);
    EXPECT_DOUBLE_EQ(coord_cart_AB[1], coord_spherical_AB_cart[1]);
}

// end of file
