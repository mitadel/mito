#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>


TEST(Functions, Sanity)
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

    // a tensor function
    auto B = f * mito::math::e_12<3> + g * mito::math::e_22<3>;

    // the tensor: f(x) * e_12 + g(x) * e_21
    auto A = 2.0 * mito::math::e_12<3> + 4.0 * mito::math::e_22<3>;

    // evaluating {B} on {x} yields {A}
    assert(B(x) == A);
}