// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


// strip the namespace
using mito::geometry::CARTESIAN;
using mito::geometry::POLAR;
using mito::geometry::SPHERICAL;


// TOFIX: this test can be made of {static_assert}s with c++26, when the
// math functions std::sin, std::cos and friends become {constexpr}


TEST(CoordinateTransformation, CartesianPolar)
{
    // create a set of coordinates
    constexpr auto coord_cart_A = mito::geometry::coordinates<CARTESIAN>({ 1.0, 1.0 });

    // transform it to polar coordinates
    auto coord_polar_A = transform_coordinates<POLAR>(coord_cart_A);

    // transform it back to cartesian coordinates
    auto coord_cart_back_A = transform_coordinates<CARTESIAN>(coord_polar_A);

    // check that we obtained the initial set of coordinates
    EXPECT_DOUBLE_EQ(coord_cart_A[0], coord_cart_back_A[0]);
    EXPECT_DOUBLE_EQ(coord_cart_A[1], coord_cart_back_A[1]);

    // create another set of coordinates
    constexpr auto coord_cart_B = mito::geometry::coordinates<CARTESIAN>({ 1.0, 0.0 });

    // transform it to polar coordinates
    auto coord_polar_B = transform_coordinates<POLAR>(coord_cart_B);

    // sum A and B in cartesian coordinates
    auto coord_cart_AB = coord_cart_A + coord_cart_B;

    // sum A and B in polar coordinates
    auto coord_polar_AB = coord_polar_A + coord_polar_B;

    // transform the cartesian sum to polar coordinates
    auto coord_cart_AB_polar = transform_coordinates<POLAR>(coord_cart_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_polar_AB[0], coord_cart_AB_polar[0]);
    EXPECT_DOUBLE_EQ(coord_polar_AB[1], coord_cart_AB_polar[1]);

    // transform the polar sum to cartesian coordinates
    auto coord_polar_AB_cart = transform_coordinates<CARTESIAN>(coord_polar_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_cart_AB[0], coord_polar_AB_cart[0]);
    EXPECT_DOUBLE_EQ(coord_cart_AB[1], coord_polar_AB_cart[1]);
}


TEST(CoordinateTransformation, CartesianSpherical)
{
    // create a set of coordinates
    constexpr auto coord_cart_A = mito::geometry::coordinates<CARTESIAN>({ 1.0, 1.0, 1.0 });

    // transform it to polar coordinates
    auto coord_spherical_A = transform_coordinates<SPHERICAL>(coord_cart_A);

    // transform it back to cartesian coordinates
    auto coord_cart_back_A = transform_coordinates<CARTESIAN>(coord_spherical_A);

    // check that we obtained the initial set of coordinates
    EXPECT_DOUBLE_EQ(coord_cart_A[0], coord_cart_back_A[0]);
    EXPECT_DOUBLE_EQ(coord_cart_A[1], coord_cart_back_A[1]);
    EXPECT_DOUBLE_EQ(coord_cart_A[2], coord_cart_back_A[2]);

    // create another set of coordinates
    constexpr auto coord_cart_B = mito::geometry::coordinates<CARTESIAN>({ 0.5, 0.0, 1.0 });

    // transform it to polar coordinates
    auto coord_spherical_B = transform_coordinates<SPHERICAL>(coord_cart_B);

    // sum A and B in cartesian coordinates
    auto coord_cart_AB = coord_cart_A + coord_cart_B;

    // sum A and B in polar coordinates
    auto coord_spherical_AB = coord_spherical_A + coord_spherical_B;

    // transform the cartesian sum to polar coordinates
    auto coord_cart_AB_spherical = transform_coordinates<SPHERICAL>(coord_cart_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_spherical_AB[0], coord_cart_AB_spherical[0]);
    EXPECT_DOUBLE_EQ(coord_spherical_AB[1], coord_cart_AB_spherical[1]);
    EXPECT_DOUBLE_EQ(coord_spherical_AB[2], coord_cart_AB_spherical[2]);

    // transform the polar sum to cartesian coordinates
    auto coord_spherical_AB_cart = transform_coordinates<CARTESIAN>(coord_spherical_AB);

    // check that we obtained the same result doing the sum in polar and in cartesian coordinates
    EXPECT_DOUBLE_EQ(coord_cart_AB[0], coord_spherical_AB_cart[0]);
    EXPECT_DOUBLE_EQ(coord_cart_AB[1], coord_spherical_AB_cart[1]);
}

// end of file
