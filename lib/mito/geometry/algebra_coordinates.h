// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_algebra_coordinates_h)
#define mito_geometry_algebra_coordinates_h


namespace mito::geometry {

    // multiplication of coordinates xA * b
    template <int D, CoordinateType coordT>
    constexpr auto operator*(const Coordinates<D, coordT> & xA, const scalar_t & b)
        -> Coordinates<D, coordT>
    {
        return b * xA;
    }

    // division of coordinates xA / b
    template <int D, CoordinateType coordT>
    constexpr auto operator/(const Coordinates<D, coordT> & xA, const scalar_t & b)
        -> Coordinates<D, coordT>
    {
        return (1.0 / b) * xA;
    }

    // operator+=
    template <int D, CoordinateType coordT>
    constexpr auto operator+=(Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB)
        -> Coordinates<D, coordT> &
    {
        xA = xA + xB;
        return xA;
    }

    // operator/=
    template <int D, CoordinateType coordT>
    constexpr auto operator/=(Coordinates<D, coordT> & xA, const scalar_t & b)
        -> Coordinates<D, coordT> &
    {
        xA = xA / b;
        return xA;
    }

    template <int D, CoordinateType coordT>
    constexpr auto distance(const Coordinates<D, coordT> & xA, const Coordinates<D, coordT> & xB)
        -> scalar_t
    {
        // return the distance between the two points
        return pyre::tensor::norm(xA - xB);
    }

}    // namespace mito


// implementation of custom {operator+}, {operator-} {operator*}
#include "algebra_cartesian_coordinates.icc"
#include "algebra_polar_coordinates.icc"
#include "algebra_spherical_coordinates.icc"

#endif    // mito_geometry_Coordinates_h

// end of file
