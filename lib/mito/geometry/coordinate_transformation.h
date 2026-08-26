// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    template <coordinates_c coordT2, coordinates_c coordT1>
    constexpr auto transform_coordinates(const coordT1 & coordinates) -> coordT2;

    template <>
    constexpr auto transform_coordinates<polar::coordinates_t, cartesian<2>::coordinates_t>(
        const cartesian<2>::coordinates_t & coordinates) -> polar::coordinates_t
    {
        auto x = coordinates[0];
        auto y = coordinates[1];
        auto r = std::sqrt(x * x + y * y);
        auto theta = std::atan2(y, x);
        return { r, theta };
    }

    template <>
    constexpr auto transform_coordinates<cartesian<2>::coordinates_t, polar::coordinates_t>(
        const polar::coordinates_t & coordinates) -> cartesian<2>::coordinates_t
    {
        auto r = coordinates[0];
        auto theta = coordinates[1];
        auto x = r * std::cos(theta);
        auto y = r * std::sin(theta);
        return { x, y };
    }

    template <>
    constexpr auto transform_coordinates<spherical::coordinates_t, cartesian<3>::coordinates_t>(
        const cartesian<3>::coordinates_t & coordinates) -> spherical::coordinates_t
    {
        auto x = coordinates[0];
        auto y = coordinates[1];
        auto z = coordinates[2];

        auto r = std::sqrt(x * x + y * y + z * z);
        auto theta = std::atan2(std::hypot(y, x), z);
        auto phi = std::atan2(y, x);

        return { r, theta, phi };
    }

    template <>
    constexpr auto transform_coordinates<cartesian<3>::coordinates_t, spherical::coordinates_t>(
        const spherical::coordinates_t & coordinates) -> cartesian<3>::coordinates_t
    {
        auto r = coordinates[0];
        auto theta = coordinates[1];
        auto phi = coordinates[2];
        auto x = r * std::sin(theta) * std::cos(phi);
        auto y = r * std::sin(theta) * std::sin(phi);
        auto z = r * std::cos(theta);
        return { x, y, z };
    }
}


// end of file
