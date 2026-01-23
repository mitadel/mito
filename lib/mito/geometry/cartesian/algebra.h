// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::geometry {

    // addition of cartesian coordinates {A} and vector {v}
    template <>
    constexpr auto operator+(const cartesian_coordinates_t<1> & A, const tensor::vector_t<1> & v)
        -> cartesian_coordinates_t<1>
    {
        // easy enough
        return cartesian_coordinates_t<1>({ A[0] + v[0] });
    }

    // subtraction of coordinates A - B
    template <>
    constexpr auto operator-(
        const cartesian_coordinates_t<1> & A, const cartesian_coordinates_t<1> & B)
        -> mito::tensor::vector_t<1>
    {
        return mito::tensor::vector_t<1>{ A[0] - B[0] };
    }


    // addition of cartesian coordinates {A} and vector {v}
    template <>
    constexpr auto operator+(const cartesian_coordinates_t<2> & A, const tensor::vector_t<2> & v)
        -> cartesian_coordinates_t<2>
    {
        // easy enough
        return cartesian_coordinates_t<2>({ A[0] + v[0], A[1] + v[1] });
    }

    // subtraction of coordinates A - B
    template <>
    constexpr auto operator-(
        const cartesian_coordinates_t<2> & A, const cartesian_coordinates_t<2> & B)
        -> mito::tensor::vector_t<2>
    {
        return mito::tensor::vector_t<2>{ A[0] - B[0], A[1] - B[1] };
    }


    // addition of cartesian coordinates {A} and vector {v}
    template <>
    constexpr auto operator+(const cartesian_coordinates_t<3> & A, const tensor::vector_t<3> & v)
        -> cartesian_coordinates_t<3>
    {
        // easy enough
        return cartesian_coordinates_t<3>({ A[0] + v[0], A[1] + v[1], A[2] + v[2] });
    }

    // subtraction of coordinates A - B
    template <>
    constexpr auto operator-(
        const cartesian_coordinates_t<3> & A, const cartesian_coordinates_t<3> & B)
        -> mito::tensor::vector_t<3>
    {
        return mito::tensor::vector_t<3>{ A[0] - B[0], A[1] - B[1], A[2] - B[2] };
    }

}


// end of file
