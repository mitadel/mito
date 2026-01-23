// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::functions {

    // functions
    template <class X, class Y>
    using function_t = Function<X, Y>;

    // the function mapping an input of type {T} to the zero scalar
    template <class T>
    [[maybe_unused]] constexpr auto zero = Constant<T, tensor::scalar_t>(0.0);

    // the function mapping an input of type {T} to the zero scalar
    template <class T>
    [[maybe_unused]] constexpr auto one = Constant<T, tensor::scalar_t>(1.0);

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
    template <subscriptable_c T, int N>
    [[maybe_unused]] constexpr auto component = Component<T, N>();

    // the linear combination
    template <typename T, function_c... Funcs>
    constexpr auto linear_combination(std::array<T, sizeof...(Funcs)> coeffs, Funcs... funcs);

    // returns the summation of functions
    template <function_c... Funcs>
    constexpr auto summation(Funcs... funcs);
}


// end of file
