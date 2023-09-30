#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>

TEST(Math, Forms)
{
    // a vector
    auto a = 2.0 * mito::math::e_0<3> + mito::math::e_1<3>;

    // the corresponding one-form
    auto a_tilda = mito::math::one_form(a);

    // a vector
    auto xi = mito::math::e_2<3>;

    // evaluating {a_tilda} on {xi} yields 0.0
    assert(a_tilda(xi) == 0.0);
}