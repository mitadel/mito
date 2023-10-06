#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>

template <int D, class F1, class F2>
constexpr auto
wedge(
    const mito::math::function_t<F1> & a_tilda,
    const mito::math::function_t<F2> &
        b_tilda)    // TOFIX: requires {a_tilda} and {b_tilda} one forms ...
{
    // return the contraction of a given vector {x} with {vector}
    return mito::math::function([ a_tilda, b_tilda ](const mito::vector_t<D> & x) -> auto{
        // return the contraction of {vector} with {x}
        return a_tilda(x) * b_tilda - b_tilda(x) * a_tilda;
    });
}

TEST(Math, Forms)
{
    // a vector
    constexpr auto a = 2.0 * mito::e_0<3> + mito::e_1<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::manifolds::one_form(a);

    // another vector
    constexpr auto b = mito::e_2<3>;

    // the corresponding one-form
    constexpr auto b_tilda = mito::manifolds::one_form(b);

    // the wedge product of {a_tilda} and {b_tilda} (a two-form)
    constexpr auto ab_two_form = wedge<3>(a_tilda, b_tilda);

    // a vector
    constexpr auto xi1 = mito::e_2<3>;

    // another vector
    constexpr auto xi2 = mito::e_1<3>;

    // contract the two-form with two vectors
    static_assert(ab_two_form(xi1)(xi2) == -1.0);
}