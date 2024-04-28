// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


// mito scalars
using mito::scalar_t;


TEST(Functions, Functor)
{
    // sqrt function
    constexpr auto sqrt = mito::functions::function([](scalar_t x) { return std::sqrt(x); });
    // power of two function
    constexpr auto pow2 = mito::functions::function([](scalar_t x) { return x * x; });

    // a given x
    constexpr auto x = 1.1;

    // check result
    static_assert(x == sqrt(pow2)(x));

    // check result
    static_assert(x == pow2(sqrt)(x));
}


// end of file
