// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_coordinate_transformation_h)
#define mito_geometry_coordinate_transformation_h


namespace mito::geometry {

    template <CoordinateType coordT2, int D, CoordinateType coordT1>
    constexpr auto transform_coordinates(const coordinates_t<D, coordT1> & coordinates)
        -> coordinates_t<D, coordT2>;

    template <>
    constexpr auto transform_coordinates<POLAR, 2, EUCLIDEAN>(
        const coordinates_t<2, EUCLIDEAN> & coordinates) -> coordinates_t<2, POLAR>
    {
        scalar_t x = coordinates[0];
        scalar_t y = coordinates[1];
        scalar_t r = std::sqrt(x * x + y * y);
        scalar_t theta = std::atan2(y, x);
        return { r, theta };
    }

    template <>
    constexpr auto transform_coordinates<EUCLIDEAN, 2, POLAR>(
        const coordinates_t<2, POLAR> & coordinates) -> coordinates_t<2, EUCLIDEAN>
    {
        scalar_t r = coordinates[0];
        scalar_t theta = coordinates[1];
        scalar_t x = r * std::cos(theta);
        scalar_t y = r * std::sin(theta);
        return { x, y };
    }
}


#endif

// end of file
