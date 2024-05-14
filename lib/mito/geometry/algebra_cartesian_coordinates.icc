// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_geometry_algebra_cartesian_coordinates_h)
#define mito_geometry_algebra_cartesian_coordinates_h


namespace mito::geometry {

    // addition of cartesian coordinates {A} and vector {v}
    template <>
    constexpr auto operator+(const Coordinates<1, CARTESIAN> & A, const vector_t<1> & v)
        -> Coordinates<1, CARTESIAN>
    {
        // easy enough
        return Coordinates<1, CARTESIAN>({ A[0] + v[0] });
    }

    // subtraction of coordinates A - B
    template <>
    constexpr auto operator-(
        const Coordinates<1, CARTESIAN> & A, const Coordinates<1, CARTESIAN> & B)
        -> mito::vector_t<1>
    {
        return mito::vector_t<1>{ A[0] - B[0] };
    }


    // addition of cartesian coordinates {A} and vector {v}
    template <>
    constexpr auto operator+(const Coordinates<2, CARTESIAN> & A, const vector_t<2> & v)
        -> Coordinates<2, CARTESIAN>
    {
        // easy enough
        return Coordinates<2, CARTESIAN>({ A[0] + v[0], A[1] + v[1] });
    }

    // subtraction of coordinates A - B
    template <>
    constexpr auto operator-(
        const Coordinates<2, CARTESIAN> & A, const Coordinates<2, CARTESIAN> & B)
        -> mito::vector_t<2>
    {
        return mito::vector_t<2>{ A[0] - B[0], A[1] - B[1] };
    }


    // addition of cartesian coordinates {A} and vector {v}
    template <>
    constexpr auto operator+(const Coordinates<3, CARTESIAN> & A, const vector_t<3> & v)
        -> Coordinates<3, CARTESIAN>
    {
        // easy enough
        return Coordinates<3, CARTESIAN>({ A[0] + v[0], A[1] + v[1], A[2] + v[2] });
    }

    // subtraction of coordinates A - B
    template <>
    constexpr auto operator-(
        const Coordinates<3, CARTESIAN> & A, const Coordinates<3, CARTESIAN> & B)
        -> mito::vector_t<3>
    {
        return mito::vector_t<3>{ A[0] - B[0], A[1] - B[1], A[2] - B[2] };
    }

}


#endif

// end of file
