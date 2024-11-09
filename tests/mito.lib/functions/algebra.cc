// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// support
#include <numbers>

// dependencies
#include <gtest/gtest.h>
#include <mito/functions.h>


// pi
using std::numbers::pi;


TEST(Algebra, ScalarValuedFunctions)
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


TEST(Algebra, TensorValuedFunctions)
{
    // the function extracting the x_0 component of a 3D vector
    constexpr auto x0 = mito::functions::x<0, 3>;
    // the function extracting the x_1 component of a 3D vector
    constexpr auto x1 = mito::functions::x<1, 3>;
    // the function extracting the x_2 component of a 3D vector
    constexpr auto x2 = mito::functions::x<2, 3>;
    // the cosine function
    constexpr auto cos = mito::functions::cos;

    // a scalar function
    constexpr auto f = x1 * x2 * cos(x0);

    // a point in space
    constexpr auto x = 2.0 * mito::e_1<3> + mito::e_2<3>;

    // evaluating {f} on {x} yields 2.0
    static_assert(f(x) == 2.0);

    // another scalar function
    auto g = f * f;

    // a tensor function (obtained by linear combination of scalar functions and tensors)
    constexpr auto B1 = f * mito::e_12<3> + g * mito::e_22<3>;

    // a tensor function
    constexpr auto B2 = mito::functions::constant<mito::vector_t<3>>(mito::e_12<3>);

    // add the two tensor functions
    constexpr auto B = B1 + B2;

    // the tensor: (f(x) + 1) * e_12 + g(x) * e_21
    constexpr auto A = 3.0 * mito::e_12<3> + 4.0 * mito::e_22<3>;

    // evaluating {B} on {x} yields {A}
    static_assert(B(x) == A);
}


TEST(Algebra, ScalarProduct)
{
    // a 2D vector
    constexpr auto x = mito::vector_t<2>{ 0.1, 1.0 };

    // the function returning the constant e0 unit vector in 2D
    constexpr auto e0 = mito::functions::constant<mito::vector_t<2>>(mito::e_0<2>);

    // check result
    static_assert(1.0 == (e0 * e0)(x));

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::x<0, 2>;
    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::x<1, 2>;

    // check result
    static_assert(0.1 == (x0 * x1)(x));
}