// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_algebra_coordinates_h)
#define mito_geometry_algebra_coordinates_h


namespace mito::geometry {

    // addition of a set of coordinates and a vector
    template <int D, CoordinateType coordT>
    constexpr auto operator+(const Coordinates<D, coordT> &, const vector_t<D> &)
        -> Coordinates<D, coordT>;

    // subtraction of two sets of coordinates (returns a vector)
    template <int D, CoordinateType coordT>
    constexpr auto operator-(const Coordinates<D, coordT> &, const Coordinates<D, coordT> &)
        -> vector_t<D>;

    // operator+=
    template <int D, CoordinateType coordT>
    constexpr auto operator+=(Coordinates<D, coordT> & A, const vector_t<D> & v)
        -> Coordinates<D, coordT> &
    {
        A = A + v;
        return A;
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
