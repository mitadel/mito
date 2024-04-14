// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_api_h)
#define mito_functions_api_h


namespace mito::functions {

    // the zero function
    constexpr auto zero = Zero();

    // the identity function
    constexpr auto one = One();

    // the power to integer function
    template <int N>
    constexpr auto pow = Power<N>();

    // the sine function
    constexpr auto sin = Sin();

    // the cosine function
    constexpr auto cos = Cos();

    // the tangent function
    constexpr auto tan = sin / cos;

    // the secant function
    constexpr auto sec = 1.0 / cos;

    // the cosecant function
    constexpr auto csec = 1.0 / sin;

    // the vector-valued vector function returning the constant D-dimensional vector that has all
    // zeros but a one component at entry N
    template <int N, int D>
    constexpr auto unit = Unit<vector_t<D>, vector_t<D>, N>();

    // the function associating to a D-dimensional x its N-th component
    template <int N, int D>
    constexpr auto x = Component<vector_t<D>, N>();
}


#endif

// end of file
