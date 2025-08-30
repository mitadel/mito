// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/functions.h>


// the coordinates functions in 2D space
static constexpr auto x0 = mito::functions::x<0, 2>;
static constexpr auto x1 = mito::functions::x<1, 2>;

// unit vectors in 3D
static constexpr auto e0 = mito::tensor::e_0<3>;
static constexpr auto e1 = mito::tensor::e_1<3>;
static constexpr auto e2 = mito::tensor::e_2<3>;

// unit matrices in 2D
static constexpr auto e00 = mito::tensor::e_00<2>;
static constexpr auto e01 = mito::tensor::e_01<2>;
static constexpr auto e10 = mito::tensor::e_10<2>;
static constexpr auto e11 = mito::tensor::e_11<2>;


TEST(Derivatives, ScalarOfScalar)
{
    // a scalar function of scalar
    constexpr auto f = mito::functions::sin;
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a scalar input
    constexpr auto x = mito::tensor::scalar_t{ 2.0 };
    // check that the derivative is correct
    static_assert(df(x) == mito::functions::cos(x));
}


TEST(Derivatives, ScalarOfVector)
{
    // a scalar function of a 2D vector
    constexpr auto f = mito::functions::sin(x0 * x1);
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a 2D vector input
    constexpr auto x = mito::tensor::vector_t<2>{ 0.1, 1.0 };
    // check the derivatives
    static_assert(df(x)[0] == (mito::functions::cos(x0 * x1) * x1)(x));
    static_assert(df(x)[1] == (mito::functions::cos(x0 * x1) * x0)(x));
}


TEST(Derivatives, ScalarOfMatrix)
{
    // a scalar function of a 2D matrix (trace)
    constexpr auto f = mito::functions::component<mito::tensor::matrix_t<2, 2>, 0>
                     + mito::functions::component<mito::tensor::matrix_t<2, 2>, 3>;
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a 2D matrix input
    constexpr auto A = mito::tensor::matrix_t<2, 2>{ 1.0, 2.0, 3.0, 4.0 };
    // check the derivatives
    static_assert(df(A)[0] == 1.0);
    static_assert(df(A)[1] == 0.0);
    static_assert(df(A)[2] == 0.0);
    static_assert(df(A)[3] == 1.0);
}


TEST(Derivatives, VectorOfScalar)
{
    // a 3D vector function of a scalar
    constexpr auto f =
        mito::functions::sin * e0 + mito::functions::cos * e1 + mito::functions::exp * e2;
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a scalar input
    constexpr auto x = mito::tensor::scalar_t{ 0.5 };
    // check the derivatives
    static_assert(df(x)[{ 0, 0 }] == mito::functions::cos(x));     // d/dx sin = cos
    static_assert(df(x)[{ 1, 0 }] == -mito::functions::sin(x));    // d/dx cos = -sin
    static_assert(df(x)[{ 2, 0 }] == mito::functions::exp(x));     // d/dx exp = exp
}


TEST(Derivatives, VectorOfVector)
{
    // a 3D vector function of a 2D vector
    constexpr auto f = x0 * e0 + x1 * e1 + x0 * x1 * e2;
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a 2D vector input
    constexpr auto x = mito::tensor::vector_t<2>{ 0.1, 0.2 };
    // check the derivatives
    static_assert(df(x)[{ 0, 0 }] == 1.0);
    static_assert(df(x)[{ 0, 1 }] == 0.0);
    static_assert(df(x)[{ 1, 0 }] == 0.0);
    static_assert(df(x)[{ 1, 1 }] == 1.0);
    static_assert(df(x)[{ 2, 0 }] == x1(x));
    static_assert(df(x)[{ 2, 1 }] == x0(x));
}


TEST(Derivatives, VectorOfMatrix)
{
    // component function to extract A_ij from a matrix A
    constexpr auto A_00 = mito::functions::component<mito::tensor::matrix_t<2, 2>, 0>;
    constexpr auto A_01 = mito::functions::component<mito::tensor::matrix_t<2, 2>, 1>;
    constexpr auto A_10 = mito::functions::component<mito::tensor::matrix_t<2, 2>, 2>;
    constexpr auto A_11 = mito::functions::component<mito::tensor::matrix_t<2, 2>, 3>;

    // a 3D vector function of a 2D matrix
    constexpr auto f = A_00 * e0 + A_01 * e1 + (A_10 - A_11) * e2;
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a 2D matrix input
    constexpr auto A = mito::tensor::matrix_t<2, 2>{ 1.0, 2.0, 3.0, 4.0 };
    // check the derivatives (first component of the output vector)
    static_assert(df(A)[{ 0, 0 }] == 1.0);    // d/dA_00 A_00 = 1
    static_assert(df(A)[{ 0, 1 }] == 0.0);    // d/dA_01 A_00 = 0
    static_assert(df(A)[{ 0, 2 }] == 0.0);    // d/dA_10 A_00 = 0
    static_assert(df(A)[{ 0, 3 }] == 0.0);    // d/dA_11 A_00 = 0
    // check the derivatives (second component of the output vector)
    static_assert(df(A)[{ 1, 0 }] == 0.0);    // d/dA_00 A_01 = 0
    static_assert(df(A)[{ 1, 1 }] == 1.0);    // d/dA_01 A_01 = 1
    static_assert(df(A)[{ 1, 2 }] == 0.0);    // d/dA_10 A_01 = 0
    static_assert(df(A)[{ 1, 3 }] == 0.0);    // d/dA_11 A_01 = 0
    // check the derivatives (third component of the output vector)
    static_assert(df(A)[{ 2, 0 }] == 0.0);     // d/dA_00 (A_10 - A_11) = 0
    static_assert(df(A)[{ 2, 1 }] == 0.0);     // d/dA_01 (A_10 - A_11) = 0
    static_assert(df(A)[{ 2, 2 }] == 1.0);     // d/dA_10 (A_10 - A_11) = 1
    static_assert(df(A)[{ 2, 3 }] == -1.0);    // d/dA_11 (A_10 - A_11) = -1
}


TEST(Derivatives, MatrixOfScalar)
{
    // a 2D matrix function of a scalar
    constexpr auto f =
        mito::functions::sin * e00 + mito::functions::cos * e01 + mito::functions::exp * e10;
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a scalar input
    constexpr auto x = mito::tensor::scalar_t{ 1.0 };
    // check the derivatives
    static_assert(df(x)[{ 0, 0 }] == mito::functions::cos(x));
    static_assert(df(x)[{ 1, 0 }] == -mito::functions::sin(x));
    static_assert(df(x)[{ 2, 0 }] == mito::functions::exp(x));
    static_assert(df(x)[{ 3, 0 }] == 0.0);
}


TEST(Derivatives, MatrixOfVector)
{
    // a 2D matrix function of a 2D vector
    constexpr auto f = mito::functions::sin(x0) * e00 + mito::functions::cos(x1) * e01
                     + mito::functions::exp(x0 * x1) * e11;
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a 2D vector input
    constexpr auto x = mito::tensor::vector_t<2>{ 1.0, -1.0 };
    // check the derivatives
    static_assert(df(x)[{ 0, 0 }] == mito::functions::cos(x0)(x));     // d/dx0 (sin(x0)) = cos(x0)
    static_assert(df(x)[{ 0, 1 }] == 0.0);                             // d/dx1 (sin(x0)) = 0
    static_assert(df(x)[{ 1, 0 }] == 0.0);                             // d/dx0 (cos(x1)) = 0
    static_assert(df(x)[{ 1, 1 }] == -mito::functions::sin(x1)(x));    // d/dx1 (cos(x1)) = -sin(x1)
    static_assert(df(x)[{ 2, 0 }] == 0.0);                             // d/dx0 (0) = 0
    static_assert(df(x)[{ 2, 1 }] == 0.0);                             // d/dx1 (0) = 0
    static_assert(
        df(x)[{ 3, 0 }]
        == (mito::functions::exp(x0 * x1) * x1)(x));    // d/dx0 (exp(x0 * x1)) = exp(x0 * x1) * x1
    static_assert(
        df(x)[{ 3, 1 }]
        == (mito::functions::exp(x0 * x1) * x0)(x));    // d/dx1 (exp(x0 * x1)) = exp(x0 * x1) * x0
}


TEST(Derivatives, MatrixOfMatrix)
{
    // a 2D matrix function of a 2D matrix
    constexpr auto f =
        (mito::functions::component<mito::tensor::matrix_t<2, 2>, 0>
         + mito::functions::component<mito::tensor::matrix_t<2, 2>, 3>) *(e00 + e10);
    // its derivative
    constexpr auto df = mito::functions::derivative(f);
    // a 2D matrix input
    constexpr auto A = mito::tensor::matrix_t<2, 2>{ 1.0, 2.0, 3.0, 4.0 };
    // check the derivatives
    static_assert(df(A)[{ 0, 0 }] == 1.0);
    static_assert(df(A)[{ 0, 1 }] == 0.0);
    static_assert(df(A)[{ 0, 2 }] == 0.0);
    static_assert(df(A)[{ 0, 3 }] == 1.0);
    static_assert(df(A)[{ 1, 0 }] == 0.0);
    static_assert(df(A)[{ 1, 1 }] == 0.0);
    static_assert(df(A)[{ 1, 2 }] == 0.0);
    static_assert(df(A)[{ 1, 3 }] == 0.0);
    static_assert(df(A)[{ 2, 0 }] == 1.0);
    static_assert(df(A)[{ 2, 1 }] == 0.0);
    static_assert(df(A)[{ 2, 2 }] == 0.0);
    static_assert(df(A)[{ 2, 3 }] == 1.0);
    static_assert(df(A)[{ 3, 0 }] == 0.0);
    static_assert(df(A)[{ 3, 1 }] == 0.0);
    static_assert(df(A)[{ 3, 2 }] == 0.0);
    static_assert(df(A)[{ 3, 3 }] == 0.0);
}


// end of file
