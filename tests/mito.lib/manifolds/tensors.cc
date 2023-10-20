#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/manifolds.h>

using mito::manifolds::_;

template <class F1, class F2>
struct overload_set : F1, F2 {
    constexpr overload_set(F1 x1, F2 x2) : F1(x1), F2(x2) {}
    using F1::operator();
    using F2::operator();
};

template <class F1, class F2>
constexpr overload_set<F1, F2>
overload(F1 x1, F2 x2)
{
    return overload_set<F1, F2>(x1, x2);
}

// example usage of {overload}
// auto f = overload(
//     [](dummy_vector) { std::cout << "call by default" << std::endl; },
//     []<typename T>(std::vector<T>) { std::cout << "call for vector" << std::endl; });

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
    // the dimension of the vector space
    constexpr int D = mito::manifolds::form_t<F1>::dim;
    auto f = overload(
        [fA, fB](mito::manifolds::dummy_vector) { return fB * fA; },
        [fA, fB](const mito::vector_t<D> & x) { return fA(x) * fB; });
    return f;
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
    static_assert(a_tensor_b(xi0)(xi1) == 1.0);

    constexpr auto contraction0 = a_tensor_b(xi0);
    static_assert(contraction0(xi1) == 1.0);

    constexpr auto contraction1 = a_tensor_b(xi0)(_);
    static_assert(contraction1(xi1) == a_tensor_b(xi0)(xi1));

    // IDEA: do that fA(_) returns fa, i.e. the non contracted form?
    constexpr auto contraction2 = a_tensor_b(_)(xi1);
    static_assert(contraction2(xi0) == a_tensor_b(xi0)(xi1));
}


// end of file
