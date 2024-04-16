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


TEST(Derivatives, HigherOrder)
{
    // pi fourths
    constexpr auto pi_fourth = 0.25 * pi;

    // a sine function
    constexpr auto sin = mito::functions::sin;
    // a cosine function
    constexpr auto cos = mito::functions::cos;

    // the first derivative of {sin}
    constexpr auto sin_i = mito::functions::derivative(sin);
    // check that it is equal to {cos}
    static_assert(cos(pi_fourth) == sin_i(pi_fourth));

    // the second derivative of {sin}
    constexpr auto sin_ii = mito::functions::derivative(sin_i);
    // check that it is equal to -{sin}
    static_assert(-sin(pi_fourth) == sin_ii(pi_fourth));

    // the third derivative of {sin}
    constexpr auto sin_iii = mito::functions::derivative(sin_ii);
    // check that it is equal to -{cos}
    static_assert(-cos(pi_fourth) == sin_iii(pi_fourth));

    // the fourth derivative of {sin}
    constexpr auto sin_iiii = mito::functions::derivative(sin_iii);
    // check that it is equal to {sin}
    static_assert(sin(pi_fourth) == sin_iiii(pi_fourth));
}


// end of file
