// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_api_h)
#define mito_functions_api_h


namespace mito::functions {

    // functions
    template <class X, class Y>
    using function_t = Function<X, Y>;

    // the function mapping an input of type {T} to the zero scalar
    template <class T>
    [[maybe_unused]] constexpr auto zero = Constant<T, scalar_t>(0.0);

    // the function mapping an input of type {T} to the zero scalar
    template <class T>
    [[maybe_unused]] constexpr auto one = Constant<T, scalar_t>(1.0);

    // the scalar power to integer function
    template <int N>
    [[maybe_unused]] constexpr auto pow = Power<N>();

    // the scalar sine function
    [[maybe_unused]] constexpr auto sin = Sin();

    // the scalar cosine function
    [[maybe_unused]] constexpr auto cos = Cos();

    // the scalar tangent function
    [[maybe_unused]] constexpr auto tan = Tan();

    // the scalar arc cosine function
    [[maybe_unused]] constexpr auto acos = ArcCos();

    // the scalar arc sine function
    [[maybe_unused]] constexpr auto asin = ArcSin();

    // the scalar arc tangent function
    [[maybe_unused]] constexpr auto atan = ArcTan();

    // the scalar exponential function
    [[maybe_unused]] constexpr auto exp = Exp();

    // the scalar natural logarithm function
    [[maybe_unused]] constexpr auto log = Log();

    // the scalar square root function
    [[maybe_unused]] constexpr auto sqrt = Sqrt();

    // the scalar cubic root function
    [[maybe_unused]] constexpr auto cbrt = Cbrt();

    // the function mapping an input of type {X} to the constant {c}
    template <class X, class Y>
    constexpr auto constant(const Y & c);

    // returns the function wrapping the functor {f}
    template <functor_c F>
    constexpr auto function(F && f);

    // the function extracting the N-th component from an input argument of type {T}
    template <class T, int N>
    constexpr auto component();

    // the function associating to a D-dimensional vector its N-th component
    template <int N, int D>
    [[maybe_unused]] constexpr auto x = component<vector_t<D>, N>();

}


#endif

// end of file
