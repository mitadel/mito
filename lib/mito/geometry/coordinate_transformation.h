// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_coordinate_transformation_h)
#define mito_geometry_coordinate_transformation_h


namespace mito::geometry {

    template <coordinates_c coordT2, coordinates_c coordT1>
    constexpr auto transform_coordinates(const coordT1 & coordinates) -> coordT2;

    template <>
    constexpr auto transform_coordinates<coordinates_t<2, POLAR>, coordinates_t<2, CARTESIAN>>(
        const coordinates_t<2, CARTESIAN> & coordinates) -> coordinates_t<2, POLAR>
    {
        auto x = coordinates[0];
        auto y = coordinates[1];
        auto r = std::sqrt(x * x + y * y);
        auto theta = std::atan2(y, x);
        return { r, theta };
    }

    template <>
    constexpr auto transform_coordinates<coordinates_t<2, CARTESIAN>, coordinates_t<2, POLAR>>(
        const coordinates_t<2, POLAR> & coordinates) -> coordinates_t<2, CARTESIAN>
    {
        auto r = coordinates[0];
        auto theta = coordinates[1];
        auto x = r * std::cos(theta);
        auto y = r * std::sin(theta);
        return { x, y };
    }

    template <>
    constexpr auto transform_coordinates<coordinates_t<3, SPHERICAL>, coordinates_t<3, CARTESIAN>>(
        const coordinates_t<3, CARTESIAN> & coordinates) -> coordinates_t<3, SPHERICAL>
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
    constexpr auto transform_coordinates<coordinates_t<3, CARTESIAN>, coordinates_t<3, SPHERICAL>>(
        const coordinates_t<3, SPHERICAL> & coordinates) -> coordinates_t<3, CARTESIAN>
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


#endif

// end of file
