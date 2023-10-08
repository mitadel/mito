#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Math, Forms)
{
    // a vector
    constexpr auto a = mito::e_0<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::manifolds::one_form(a);

    // another vector
    constexpr auto b = mito::e_1<3>;

    // the corresponding one-form
    constexpr auto b_tilda = mito::manifolds::one_form(b);

    // the wedge product of {a_tilda} and {b_tilda} (a two-form)
    constexpr auto ab_two_form = wedge(a_tilda, b_tilda);

    // a vector
    constexpr auto xi0 = mito::e_0<3>;

    // another vector
    constexpr auto xi1 = mito::e_1<3>;

    // contract the two-form with two vectors
    static_assert(ab_two_form(xi0)(xi1) == 1.0);

    // check anti-symmetry property
    static_assert(ab_two_form(xi0)(xi1) == -ab_two_form(xi1)(xi0));

}