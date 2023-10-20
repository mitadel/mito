#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>

using mito::manifolds::_;

// construct a one-form based on its metric-equivalent vector
template <int D>
constexpr auto
one_form(mito::vector_t<D> vector)
{
    // return a one-form that, when contracted with {x}...
    return mito::manifolds::form([vector](const mito::vector_t<D> & x) -> mito::scalar_t {
        // ... returns the contraction of {vector} with {x}
        return vector * x;
    });
}

// tensor product of forms
template <class F1, class F2>
constexpr auto
tens(const mito::manifolds::form_t<F1> & fA, const mito::manifolds::form_t<F2> & fB)
{
    return mito::manifolds::tensor(
        [fA, fB]<class xA, class xB>(const xA & x, const xB & y) { return fA(x) * fB(y); });
}


TEST(Tensors, Base)
{
    // a vector field
    constexpr auto a = mito::e_0<3>;

    // the corresponding one-form
    constexpr auto a_tilda = one_form(a);

    // another vector field
    constexpr auto b = mito::e_1<3>;

    // the corresponding one-form
    constexpr auto b_tilda = one_form(b);

    // tensor product of two one-forms
    constexpr auto a_tensor_b = tens(a_tilda, b_tilda);

    // a vector
    constexpr auto xi0 = mito::e_0<3>;

    // another vector
    constexpr auto xi1 = mito::e_1<3>;

    // check result of double contraction
    static_assert(a_tensor_b(xi0, xi1) == a_tilda(xi0) * b_tilda(xi1));

    // contract first index
    constexpr auto contraction1 = a_tensor_b(xi0, _);
    static_assert(contraction1(xi1) == a_tensor_b(xi0, xi1));

    // contract second index
    constexpr auto contraction2 = a_tensor_b(_, xi1);
    static_assert(contraction2(xi0) == a_tensor_b(xi0, xi1));
}


// end of file
