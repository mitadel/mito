// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/geometry.h>


TEST(Coordinates, Cartesian)
{
    // cartesian coordinates in 2D
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::CARTESIAN>;

    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::coordinates<coordinates_t>({ 0.0, 1.0 });
    constexpr auto coord_B = mito::geometry::coordinates<coordinates_t>({ 1.0, 0.0 });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, POLAR)
{
    // polar coordinates in 2D
    using coordinates_t = mito::geometry::coordinates_t<2, mito::geometry::POLAR>;

    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::coordinates<coordinates_t>({ 1.0, 0.0 });
    constexpr auto coord_B =
        mito::geometry::coordinates<coordinates_t>({ 1.0, 0.5 * std::numbers::pi });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, SPHERICAL_Z)
{
    // spherical coordinates in 3D
    using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::SPHERICAL>;

    // create two sets of coordinates
    constexpr auto coord_A =
        mito::geometry::coordinates<coordinates_t>({ 1.0, 0.5 * std::numbers::pi, 0.0 });
    constexpr auto coord_B = mito::geometry::coordinates<coordinates_t>(
        { 1.0, 0.5 * std::numbers::pi, 0.5 * std::numbers::pi });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, SPHERICAL_Y)
{
    // spherical coordinates in 3D
    using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::SPHERICAL>;

    // create two sets of coordinates
    constexpr auto coord_A =
        mito::geometry::coordinates<coordinates_t>({ 1.0, 0.25 * std::numbers::pi, 0.0 });
    constexpr auto coord_B =
        mito::geometry::coordinates<coordinates_t>({ 1.0, 0.75 * std::numbers::pi, 0.0 });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, SPHERICAL_X)
{
    // spherical coordinates in 3D
    using coordinates_t = mito::geometry::coordinates_t<3, mito::geometry::SPHERICAL>;

    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::coordinates<coordinates_t>(
        { 1.0, 0.25 * std::numbers::pi, 0.5 * std::numbers::pi });
    constexpr auto coord_B = mito::geometry::coordinates<coordinates_t>(
        { 1.0, 0.75 * std::numbers::pi, 0.5 * std::numbers::pi });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


// end of file
