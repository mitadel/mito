// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;
// mito scalars
using mito::tensor::scalar_t;


TEST(Derivatives, FunctionPlusConstant)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the constant plus the function
    constexpr auto a_sin = a + sin;
    // the function plus the constant
    constexpr auto sin_a = sin + a;
    // check results
    static_assert(a + sin(pi_sixth) == a_sin(pi_sixth));
    static_assert(a + sin(pi_sixth) == sin_a(pi_sixth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto a_sin_i = mito::functions::derivative(a_sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // check results
    static_assert(sin_i(pi_sixth) == a_sin_i(pi_sixth));
    static_assert(sin_i(pi_sixth) == sin_a_i(pi_sixth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {a_sin}
    constexpr auto a_sin_ii = mito::functions::derivative(a_sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // check results
    static_assert(sin_ii(pi_sixth) == a_sin_ii(pi_sixth));
    static_assert(sin_ii(pi_sixth) == sin_a_ii(pi_sixth));
}


TEST(Derivatives, FunctionTimesConstant)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the constant times function product
    constexpr auto a_sin = a * sin;
    // the function times constant product
    constexpr auto sin_a = sin * a;
    // check results
    static_assert(a * sin(pi_sixth) == a_sin(pi_sixth));
    static_assert(a * sin(pi_sixth) == sin_a(pi_sixth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto a_sin_i = mito::functions::derivative(a_sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // check results
    static_assert(a * sin_i(pi_sixth) == a_sin_i(pi_sixth));
    static_assert(a * sin_i(pi_sixth) == sin_a_i(pi_sixth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {a_sin}
    constexpr auto a_sin_ii = mito::functions::derivative(a_sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // check results
    static_assert(a * sin_ii(pi_sixth) == a_sin_ii(pi_sixth));
    static_assert(a * sin_ii(pi_sixth) == sin_a_ii(pi_sixth));
}


TEST(Derivatives, FunctionDividedByConstant)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the function times constant product
    constexpr auto sin_a = sin / a;
    // check result
    static_assert(sin(pi_sixth) / a == sin_a(pi_sixth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // check result
    static_assert(sin_i(pi_sixth) / a == sin_a_i(pi_sixth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // check result
    static_assert(sin_ii(pi_sixth) / a == sin_a_ii(pi_sixth));
}


TEST(Derivatives, FunctionMinusConstant)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a constant
    constexpr auto a = 0.1;
    // the constant minus the function
    constexpr auto a_sin = a - sin;
    // the function minus the constant
    constexpr auto sin_a = sin - a;
    // check results
    static_assert(a - sin(pi_sixth) == a_sin(pi_sixth));
    static_assert(sin(pi_sixth) - a == sin_a(pi_sixth));

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // the first derivative of {a_sin}
    constexpr auto a_sin_i = mito::functions::derivative(a_sin);
    // the first derivative of {a_sin}
    constexpr auto sin_a_i = mito::functions::derivative(sin_a);
    // check results
    static_assert(-sin_i(pi_sixth) == a_sin_i(pi_sixth));
    static_assert(sin_i(pi_sixth) == sin_a_i(pi_sixth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // the second derivative of {a_sin}
    constexpr auto a_sin_ii = mito::functions::derivative(a_sin_i);
    // the first derivative of {a_sin}
    constexpr auto sin_a_ii = mito::functions::derivative(sin_a_i);
    // check results
    static_assert(-sin_ii(pi_sixth) == a_sin_ii(pi_sixth));
    static_assert(sin_ii(pi_sixth) == sin_a_ii(pi_sixth));
}


TEST(Derivatives, Zero)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // zero function
    constexpr auto zero = mito::functions::zero<scalar_t>;
    // check result
    static_assert(0.0 == zero(pi_sixth));

    // the first derivative of {zero}
    constexpr auto zero_i = mito::functions::derivative(zero);
    // check result
    static_assert(zero(pi_sixth) == zero_i(pi_sixth));

    // the second derivative of {zero}
    constexpr auto zero_ii = mito::functions::derivative(zero_i);
    // check result
    static_assert(zero(pi_sixth) == zero_ii(pi_sixth));
}


TEST(Derivatives, One)
{
    // pi sixths
    constexpr auto pi_sixth = pi / 6.0;

    // identity function
    constexpr auto one = mito::functions::one<scalar_t>;
    // check result
    static_assert(1.0 == one(pi_sixth));

    // the first derivative of {one}
    constexpr auto one_i = mito::functions::derivative(one);
    // zero function
    constexpr auto zero = mito::functions::zero<scalar_t>;
    // check result
    static_assert(zero(pi_sixth) == one_i(pi_sixth));

    // the second derivative of {one}
    constexpr auto one_ii = mito::functions::derivative(one_i);
    // check result
    static_assert(zero(pi_sixth) == one_ii(pi_sixth));
}


// end of file
