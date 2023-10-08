#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>


TEST(Forms, TensorProduct)
{
    // a vector
    constexpr auto a = mito::e_0<3>;

    // the corresponding one-form
    constexpr auto a_tilda = mito::manifolds::one_form(a);

    // another vector
    constexpr auto b = mito::e_1<3>;

    // the corresponding one-form
    constexpr auto b_tilda = mito::manifolds::one_form(b);

    // tensor product of two one-forms
    constexpr auto a_tensor_b = a_tilda * b_tilda;

    // a vector
    constexpr auto xi0 = mito::e_0<3>;

    // another vector
    constexpr auto xi1 = mito::e_1<3>;

    // check result of double contraction
    static_assert(a_tensor_b(xi0)(xi1) == 1.0);
    static_assert(a_tensor_b(xi1)(xi0) == 0.0);

    // another vector
    constexpr auto c = mito::e_2<3>;

    // the corresponding one-form
    constexpr auto c_tilda = mito::manifolds::one_form(c);

    // triple tensor product
    constexpr auto a_tensor_b_tensor_c = a_tilda * b_tilda * c_tilda;

    // another vector
    constexpr auto xi2 = mito::e_2<3>;

    // check result of triple contraction
    static_assert(a_tensor_b_tensor_c(xi0)(xi1)(xi2) == 1.0);
    static_assert(a_tensor_b_tensor_c(xi2)(xi0)(xi1) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi1)(xi2)(xi0) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi0)(xi2)(xi1) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi2)(xi1)(xi0) == 0.0);
    static_assert(a_tensor_b_tensor_c(xi1)(xi0)(xi2) == 0.0);
}


// end of file
