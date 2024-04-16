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


// pi fourths
static constexpr auto pi_fourth = 0.25 * pi;


TEST(DerivativesInverse, SqrtPow)
{
    // sqrt function
    constexpr auto sqrt = mito::functions::sqrt;
    // power of two function
    constexpr auto pow2 = mito::functions::pow<2>;

    // the composition of the function with its inverse
    constexpr auto sqrt_pow2 = sqrt(pow2);
    // the derivative of sqrt_pow2
    constexpr auto sqrt_pow2_i = mito::functions::derivative(sqrt_pow2);
    // check result
    static_assert(1.0 == sqrt_pow2_i(pi_fourth));
}


TEST(DerivativesInverse, CbrtPow)
{
    // cbrt function
    constexpr auto cbrt = mito::functions::cbrt;
    // power of three function
    constexpr auto pow3 = mito::functions::pow<3>;

    // the composition of the function with its inverse
    constexpr auto cbrt_pow3 = cbrt(pow3);
    // the derivative of sqrt_pow2
    constexpr auto cbrt_pow3_i = mito::functions::derivative(cbrt_pow3);
    // check result
    static_assert(1.0 == cbrt_pow3_i(pi_fourth));
}


TEST(DerivativesInverse, CosACos)
{
    // cos function
    constexpr auto cos = mito::functions::cos;
    // acos function
    constexpr auto acos = mito::functions::acos;

    // the composition of the function with its inverse
    constexpr auto cos_acos = cos(acos);
    // the derivative of cos_acos
    constexpr auto cos_acos_i = mito::functions::derivative(cos_acos);
    // check result
    static_assert(1.0 == cos_acos_i(pi_fourth));
}


TEST(DerivativesInverse, SinASin)
{
    // sin function
    constexpr auto sin = mito::functions::sin;
    // asin function
    constexpr auto asin = mito::functions::asin;

    // the composition of the function with its inverse
    constexpr auto sin_asin = sin(asin);
    // the derivative of sin_asin
    constexpr auto sin_asin_i = mito::functions::derivative(sin_asin);
    // check result
    static_assert(1.0 == sin_asin_i(pi_fourth));
}


TEST(DerivativesInverse, TanATan)
{
    // tan function
    constexpr auto tan = mito::functions::tan;
    // atan function
    constexpr auto atan = mito::functions::atan;

    // the composition of the function with its inverse
    constexpr auto tan_atan = tan(atan);
    // the derivative of tan_atan
    constexpr auto tan_atan_i = mito::functions::derivative(tan_atan);
    // check result
    static_assert(1.0 == tan_atan_i(pi_fourth));
}


TEST(DerivativesInverse, ExpLog)
{
    // exp function
    constexpr auto exp = mito::functions::exp;
    // log function
    constexpr auto log = mito::functions::log;

    // the composition of the function with its inverse
    constexpr auto exp_log = exp(log);
    // the derivative of exp_log
    constexpr auto exp_log_i = mito::functions::derivative(exp_log);
    // check result
    static_assert(1.0 == exp_log_i(pi_fourth));
}


// end of file
