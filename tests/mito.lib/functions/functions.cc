// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;


TEST(Functions, Algebra)
{
    // a vector
    constexpr mito::vector_t<2> x = { 1.0, pi };

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::x<0, 2>;

    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;

    // a scalar-valued function
    constexpr auto function1 = mito::functions::cos(x0 * x1);

    // a scalar-valued function
    constexpr auto function2 = 5.0 * mito::functions::one<mito::vector_t<2>>;

    // the sum of the two scalar functions
    constexpr auto function3 = function1 + function2;

    // do some math
    constexpr auto function4 = function3 + function3 + function3;
    constexpr auto function5 = 3.0 * function3;
    // check result
    static_assert(function4(x) == function5(x));

    constexpr auto function6 = function4 - function3;
    constexpr auto function7 = function5 - function3;
    static_assert(function6(x) == function7(x));

    constexpr auto function8 = (function7 * 0.5) / 0.5;
    static_assert(function7(x) == function8(x));

    constexpr auto function9 = (0.5 / function7) * function7;
    static_assert(function9(x) == 0.5);

    constexpr auto function10 = (pi + function7) - function7;
    static_assert(function10(x) == pi);

    constexpr auto function11 = pi + function7 - 2.0 * pi;
    static_assert(function11(x) == function7(x) - pi);

    // (cos(xy) + 5) / cos(xy)
    constexpr auto function12 = (function1 + function2) / function1;
    static_assert((function1(x) + function2(x)) / function1(x) == function12(x));

    constexpr auto e0 = mito::functions::constant<mito::vector_t<2>>(mito::e_0<3>);
    constexpr auto e1 = mito::functions::constant<mito::vector_t<2>>(mito::e_1<3>);
    constexpr auto e2 = mito::functions::constant<mito::vector_t<2>>(mito::e_2<3>);

    // a vector function
    constexpr auto function16 = function1 * e0 + function1 * e1 + function1 * e2;

    // vector times scalar multiplication
    constexpr mito::real alpha = 10;
    constexpr auto function17 = alpha * function16;
    static_assert(function17(x) == alpha * function16(x));

    // inner product between vectors
    constexpr mito::vector_t<3> my_vector = { 1, 2, 3 };
    constexpr auto function18 = my_vector * function16;
    static_assert(function18(x) == -6);
}