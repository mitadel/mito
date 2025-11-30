// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/functions.h>


TEST(VectorFunctions, Components)
{
    // vectors in 2D
    using vector_t = mito::tensor::vector_t<2>;

    // a 2D vector
    constexpr auto x = vector_t{ 0.1, 1.0 };

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::component<vector_t, 0>;
    // check result
    static_assert(0.1 == x0(x));

    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::component<vector_t, 1>;
    // check result
    static_assert(1.0 == x1(x));

    // the partial derivatives of x0
    constexpr auto dx0 = mito::functions::derivative(x0);

    // check result
    static_assert(1.0 == dx0(x)[0]);
    static_assert(0.0 == dx0(x)[1]);

    // the partial derivatives of x1
    constexpr auto dx1 = mito::functions::derivative(x1);
    // check result
    static_assert(0.0 == dx1(x)[0]);
    static_assert(1.0 == dx1(x)[1]);

    // the second partial derivatives of x0
    constexpr auto ddx0 = mito::functions::derivative(dx0);

    // check result
    static_assert(0.0 == ddx0(x)[{ 0, 0 }]);
    static_assert(0.0 == ddx0(x)[{ 0, 1 }]);
    static_assert(0.0 == ddx0(x)[{ 1, 0 }]);
    static_assert(0.0 == ddx0(x)[{ 1, 1 }]);

    // the second partial derivatives of x1
    constexpr auto ddx1 = mito::functions::derivative(dx1);
    // check result
    static_assert(0.0 == ddx1(x)[{ 0, 0 }]);
    static_assert(0.0 == ddx1(x)[{ 0, 1 }]);
    static_assert(0.0 == ddx1(x)[{ 1, 0 }]);
    static_assert(0.0 == ddx1(x)[{ 1, 1 }]);
}


TEST(Derivatives, PartialDerivatives)
{
    // vectors in 2D
    using vector_t = mito::tensor::vector_t<2>;

    // a 2D vector
    constexpr auto x = vector_t{ 0.1, 1.0 };

    // the function extracting the x_0 component
    constexpr auto x0 = mito::functions::component<vector_t, 0>;
    // the function extracting the x_1 component
    constexpr auto x1 = mito::functions::component<vector_t, 1>;

    // sin(x0 * x1)
    constexpr auto sin = mito::functions::sin(x0 * x1);

    // the partial derivatives of sin(x0 * x1) wrt to x0 and x1
    constexpr auto dsin = mito::functions::derivative(sin);

    // cos(x0 * x1)
    constexpr auto cos = mito::functions::cos(x0 * x1);

    // check result
    static_assert((cos * x1)(x) == dsin[0](x));
    static_assert((cos * x0)(x) == dsin[1](x));
}


// end of file
