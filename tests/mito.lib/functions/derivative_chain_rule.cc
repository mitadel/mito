// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;
// mito scalars
using mito::scalar_t;


TEST(Derivatives, Composition)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // sine function
    constexpr auto sin = mito::functions::sin;
    // power of two function
    constexpr auto pow2 = mito::functions::pow<2>;

    // the function sin(pow<2>)
    constexpr auto sin_pow2 = sin(pow2);
    // check result
    static_assert(sin(pow2(pi_fourth)) == sin_pow2(pi_fourth));

    // the derivative of sin(pow<2>)
    constexpr auto sin_pow2_i = mito::functions::derivative(sin_pow2);
    // cosine function
    constexpr auto cos = mito::functions::cos;
    // power of one function
    constexpr auto pow1 = mito::functions::pow<1>;
    // check result
    static_assert(2.0 * cos(pow2(pi_fourth)) * pow1(pi_fourth) == sin_pow2_i(pi_fourth));

    // the function pow<2>(sin)
    constexpr auto pow2_sin = pow2(sin);
    // check result
    static_assert(pow2(sin(pi_fourth)) == pow2_sin(pi_fourth));

    // the derivative of sin(pow<2>)
    constexpr auto pow2_sin_i = mito::functions::derivative(pow2_sin);
    // check result
    static_assert(2.0 * pow1(sin(pi_fourth)) * cos(pi_fourth) == pow2_sin_i(pi_fourth));
}


// end of file
