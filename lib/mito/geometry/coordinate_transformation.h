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
        scalar_t x = coordinates[0];
        scalar_t y = coordinates[1];
        scalar_t r = std::sqrt(x * x + y * y);
        scalar_t theta = std::atan2(y, x);
        return { r, theta };
    }

    template <>
    constexpr auto transform_coordinates<coordinates_t<2, CARTESIAN>, coordinates_t<2, POLAR>>(
        const coordinates_t<2, POLAR> & coordinates) -> coordinates_t<2, CARTESIAN>
    {
        scalar_t r = coordinates[0];
        scalar_t theta = coordinates[1];
        scalar_t x = r * std::cos(theta);
        scalar_t y = r * std::sin(theta);
        return { x, y };
    }

    template <>
    constexpr auto transform_coordinates<coordinates_t<3, SPHERICAL>, coordinates_t<3, CARTESIAN>>(
        const coordinates_t<3, CARTESIAN> & coordinates) -> coordinates_t<3, SPHERICAL>
    {
        scalar_t x = coordinates[0];
        scalar_t y = coordinates[1];
        scalar_t z = coordinates[2];

        scalar_t r = std::sqrt(x * x + y * y + z * z);
        mito::scalar_t theta = std::atan2(std::hypot(y, x), z);
        mito::scalar_t phi = std::atan2(y, x);

        return { r, theta, phi };
    }

    template <>
    constexpr auto transform_coordinates<coordinates_t<3, CARTESIAN>, coordinates_t<3, SPHERICAL>>(
        const coordinates_t<3, SPHERICAL> & coordinates) -> coordinates_t<3, CARTESIAN>
    {
        scalar_t r = coordinates[0];
        scalar_t theta = coordinates[1];
        scalar_t phi = coordinates[2];
        scalar_t x = r * std::sin(theta) * std::cos(phi);
        scalar_t y = r * std::sin(theta) * std::sin(phi);
        scalar_t z = r * std::cos(theta);
        return { x, y, z };
    }
}


#endif

// end of file
