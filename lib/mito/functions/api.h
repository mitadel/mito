// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_api_h)
#define mito_functions_api_h


// TODO: define this in terms of {std::tan} directly (and derivative)
// {cmath} supported functions


namespace mito::functions {

    // the function mapping an input of type {T} to the zero scalar
    template <tensor_or_scalar_c T>
    [[maybe_unused]] constexpr auto zero = Constant<T, scalar_t>(0.0);

    // the function mapping an input of type {T} to the zero scalar
    template <tensor_or_scalar_c T>
    [[maybe_unused]] constexpr auto one = Constant<T, scalar_t>(1.0);

    // the scalar power to integer function
    template <int N>
    [[maybe_unused]] constexpr auto pow = Power<N>();

    // the scalar sine function
    [[maybe_unused]] constexpr auto sin = Sin();

    // the scalar cosine function
    [[maybe_unused]] constexpr auto cos = Cos();

    // the scalar tangent function
    [[maybe_unused]] constexpr auto tan = sin / cos;

    // the function mapping an input of type {X} to the constant {c}
    template <tensor_or_scalar_c X, tensor_or_scalar_c Y>
    [[maybe_unused]] constexpr auto constant(const Y & c);

    // the function associating to a D-dimensional vector its N-th component
    template <int N, int D>
    [[maybe_unused]] constexpr auto x = Component<vector_t<D>, N>();
}


#endif

// end of file
