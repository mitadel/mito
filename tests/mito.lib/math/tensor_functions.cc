#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>


TEST(Math, TensorFunctions)
{
    // a scalar function
    auto f = mito::math::function(
        [](const mito::vector_t<3> & x) -> mito::scalar_t { return x[1] * x[2] * cos(x[0]); });

    // a point in space
    auto x = 2.0 * mito::math::e_1<3> + mito::math::e_2<3>;

    // evaluating {f} on {x} yields 2.0
    assert(f(x) == 2.0);

    // another scalar function
    auto g = f * f;

    // a tensor function (obtained by linear combination of scalar functions and tensors)
    auto B1 = f * mito::math::e_12<3> + g * mito::math::e_22<3>;

    // a tensor function
    auto B2 = mito::math::function(
        [](const mito::vector_t<3> &) -> mito::matrix_t<3> { return mito::math::e_12<3>; });

    // add the two tensor functions
    auto B = B1 + B2;

    // the tensor: (f(x) + 1) * e_12 + g(x) * e_21
    auto A = 3.0 * mito::math::e_12<3> + 4.0 * mito::math::e_22<3>;

    // evaluating {B} on {x} yields {A}
    assert(B(x) == A);
}