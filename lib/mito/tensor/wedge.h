// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_tensor_wedge_h)
#define mito_tensor_wedge_h


// wedge operator on forms
namespace mito::tensor {

    // the wedge product of one one-form (trivial case)
    constexpr auto wedge(const one_form_c auto & a_tilda)
    {
        // return a {a_tilda}
        return a_tilda;
    }

    // wedge of a scalar with a p-form
    template <int P, class F>
    constexpr auto wedge(const real & a, const form_t<P, F> & a_tilda)
    {
        return a * a_tilda;
    }

    // wedge of a scalar with a p-form
    template <int P, class F>
    constexpr auto wedge(const form_t<P, F> & a_tilda, const real & a)
    {
        return a * a_tilda;
    }

    // the wedge product of two one-forms
    constexpr auto wedge(const one_form_c auto & a_tilda, const one_form_c auto & b_tilda)
    {
        // return a {form} that, when contracted with {x} and {y}...
        return tensor(a_tilda, b_tilda) - tensor(b_tilda, a_tilda);
    }

    // the wedge product of three one-forms
    constexpr auto wedge(
        const one_form_c auto & a_tilda, const one_form_c auto & b_tilda,
        const one_form_c auto & c_tilda)
    {
        // return a {form} that, when contracted with {x}, {y} and {z}...
        return form<3>(
            [a_tilda, b_tilda,
             c_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y, vector_or_dummy_c Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * wedge(b_tilda, c_tilda)(y, z)
                     + b_tilda(x) * wedge(c_tilda, a_tilda)(y, z)
                     + c_tilda(x) * wedge(a_tilda, b_tilda)(y, z);
            });
    }

    // TOFIX: this wedge product could be implemented in general for any two p1- and p2-forms
    // in a similar fashion to the {split_variadic} example in the sandbox, as the sum of two
    // (p1+p2)-forms where the first contracts the firs p1 parameters (...) with b_tilda and the
    // rest with wedge(b_tilda(...), a_tilda), while the second contracts the first p2 parameters
    // (...) with a_tilda and the rest with wedge(b_tilda, a_tilda(...))

    // wedge of a p1-form with a p2-form
    template <int P1, class F1, int P2, class F2>
    constexpr auto wedge(const form_t<P1, F1> & b_tilda, const form_t<P2, F2> & a_tilda)
    requires(P1 == 1 && P2 == 2)
    {
        // positive sign if P1 is even, negative sign if P1 is odd
        constexpr int sign = (P1 % 2 ? +1 : -1);

        return form<P1 + P2>(
                   [a_tilda, b_tilda]<class X, class Y, class Z>(
                       const X & x, const Y & y, const Z & z) -> auto {
                       return wedge(b_tilda(x), a_tilda)(y, z);
                   })
             + sign
                   * form<P1 + P2>(
                       [a_tilda, b_tilda]<class X, class Y, class Z>(
                           const X & x, const Y & y, const Z & z) -> auto {
                           return wedge(b_tilda, a_tilda(x, y))(z);
                       });
    }
}


#endif

// end of file
