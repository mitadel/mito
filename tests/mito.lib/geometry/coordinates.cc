// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/geometry.h>


TEST(Coordinates, Cartesian)
{
    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::cartesian::coordinates({ 0.0, 2.0 });
    constexpr auto coord_B = mito::geometry::cartesian::coordinates({ 2.0, 0.0 });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, POLAR)
{
    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::polar::coordinates({ 2.0, 0.0 });
    constexpr auto coord_B = mito::geometry::polar::coordinates({ 2.0, 0.5 * std::numbers::pi });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, SPHERICAL_Z)
{
    // create two sets of coordinates
    constexpr auto coord_A =
        mito::geometry::spherical::coordinates({ 2.0, 0.5 * std::numbers::pi, 0.0 });
    constexpr auto coord_B = mito::geometry::spherical::coordinates(
        { 2.0, 0.5 * std::numbers::pi, 0.5 * std::numbers::pi });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, SPHERICAL_Y)
{
    // create two sets of coordinates
    constexpr auto coord_A =
        mito::geometry::spherical::coordinates({ 2.0, 0.25 * std::numbers::pi, 0.0 });
    constexpr auto coord_B =
        mito::geometry::spherical::coordinates({ 2.0, 0.75 * std::numbers::pi, 0.0 });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


TEST(Coordinates, SPHERICAL_X)
{
    // create two sets of coordinates
    constexpr auto coord_A = mito::geometry::spherical::coordinates(
        { 2.0, 0.25 * std::numbers::pi, 0.5 * std::numbers::pi });
    constexpr auto coord_B = mito::geometry::spherical::coordinates(
        { 2.0, 0.75 * std::numbers::pi, 0.5 * std::numbers::pi });

    // sanity check
    static_assert(coord_A + (coord_B - coord_A) == coord_B);
}


// end of file
