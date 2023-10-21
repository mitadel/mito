#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Forms, WedgeProduct)
{
    // the euclidean metric in 3D space
    constexpr auto metric = mito::identity<3>;

    // a vector field
    constexpr auto a = mito::e_0<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::manifolds::one_form(a, metric);

    // another vector field
    constexpr auto b = mito::e_1<3>;

    // the corresponding one-form
    constexpr auto b_tilda = mito::manifolds::one_form(b, metric);

    // the wedge product of {a_tilda} and {b_tilda} (a two-form)
    constexpr auto ab_two_form = mito::manifolds::wedge(a_tilda, b_tilda);

    // a vector
    constexpr auto xi0 = mito::e_0<3>;

    // another vector
    constexpr auto xi1 = mito::e_1<3>;

    // contract the two-form with two vectors
    static_assert(ab_two_form(xi0, xi1) == 1.0);

    // check anti-symmetry property
    static_assert(ab_two_form(xi0, xi1) == -ab_two_form(xi1, xi0));

    // another vector field
    constexpr auto c = mito::e_2<3>;

    // the corresponding one-form
    constexpr auto c_tilda = mito::manifolds::one_form(c, metric);

    // the wedge product of {ab_two_form} and {c_tilda} (a three-form)
    constexpr auto abc_three_form = mito::manifolds::wedge(a_tilda, b_tilda, c_tilda);

    // another vector
    constexpr auto xi2 = mito::e_2<3>;

    // contract the three-form with three vectors
    static_assert(abc_three_form(xi0, xi1, xi2) == 1.0);

    // check anti-symmetry property
    static_assert(abc_three_form(xi0, xi1, xi2) == -abc_three_form(xi0, xi2, xi1));
    static_assert(abc_three_form(xi1, xi2, xi0) == -abc_three_form(xi1, xi0, xi2));
    static_assert(abc_three_form(xi2, xi0, xi1) == -abc_three_form(xi2, xi1, xi0));
}