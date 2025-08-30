// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;


TEST(Derivatives, Subscript)
{
    // a matrix
    constexpr auto x = pi / 4.0;

    // a vector function R -> R^2x3
    constexpr auto f = mito::functions::sin * mito::tensor::e_12<2, 3>;

    // check that evaluating and subscripting is the same than subscripting and evaluating
    static_assert(f(x)[5] == f[5](x));

    // check that the derivative of the subscript is the subscript of the derivative
    static_assert(mito::functions::derivative(f[5])(x) == mito::functions::derivative(f)(x)[5]);
}


// end of file
