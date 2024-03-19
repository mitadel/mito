// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

    // addition of forms fa + fb
    template <int P, class F1, class F2>
    constexpr auto operator+(const form_t<P, F1> & fA, const form_t<P, F2> & fB)
    requires(P > 1)
    {
        return form<P>(
            [fA, fB]<typename... argsT>(argsT... args) { return fA(args...) + fB(args...); });
    }

    // scalar * form
    template <int P, class F>
    constexpr auto operator*(const real & a, const form_t<P, F> & f)
    requires(P > 1)
    {
        return form<P>([a, f]<typename... argsT>(argsT... args) { return a * f(args...); });
    }

    // addition of forms fa + fb (specialization for one-forms)
    template <class F1, class F2>
    constexpr auto operator+(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    {
        return one_form([fA, fB]<class X>(const X & x) { return fA(x) + fB(x); });
    }

    // product of forms fa * fb (returns the tensor product of one-forms)
    template <class F1, class F2>
    constexpr auto operator*(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    {
        return tensor(fA, fB);
    }

    // scalar * form (specialization for one-forms)
    template <class F>
    constexpr auto operator*(const real & a, const one_form_t<F> & f)
    {
        return one_form([a, f]<class X>(const X & x) { return a * f(x); });
    }

    // form * scalar
    template <int P, class F>
    constexpr auto operator*(const form_t<P, F> & f, const real & a)
    {
        return a * f;
    }

    // unary operator- for forms
    template <int P, class F>
    constexpr auto operator-(const form_t<P, F> & f)
    {
        return -1.0 * f;
    }

    // subtraction of forms fa - fb
    template <int P, class F1, class F2>
    constexpr auto operator-(const form_t<P, F1> & fA, const form_t<P, F2> & fB)
    {
        return fA + (-fB);
    }

    // the wedge product of one one-form (trivial case)
    template <class F1>
    constexpr auto wedge(const one_form_t<F1> & a_tilda)
    {
        // return a {a_tilda}
        return a_tilda;
    }

    // the wedge product of two one-forms
    template <class F1, class F2>
    constexpr auto wedge(const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda)
    {
        return tensor(a_tilda, b_tilda) - tensor(b_tilda, a_tilda);
    }

    // the wedge product of three one-forms
    template <class F1, class F2, class F3>
    constexpr auto wedge(
        const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda,
        const one_form_t<F3> & c_tilda)
    {
        // return a {form} that, when contracted with {x}, {y} and {z}...
        return mito::manifolds::form<3>(
            [a_tilda, b_tilda, c_tilda]<class X, class Y, class Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * wedge(b_tilda, c_tilda)(y, z)
                     + b_tilda(x) * wedge(c_tilda, a_tilda)(y, z)
                     + c_tilda(x) * wedge(a_tilda, b_tilda)(y, z);
            });
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

        return mito::manifolds::form<P1 + P2>(
                   [a_tilda, b_tilda]<class X, class Y, class Z>(
                       const X & x, const Y & y, const Z & z) -> auto {
                       return wedge(b_tilda(x), a_tilda)(y, z);
                   })
             + sign
                   * mito::manifolds::form<P1 + P2>(
                       [a_tilda, b_tilda]<class X, class Y, class Z>(
                           const X & x, const Y & y, const Z & z) -> auto {
                           return wedge(b_tilda, a_tilda(x, y))(z);
                       });
    }

    // the tensor product of two one-forms
    template <class F1, class F2>
    constexpr auto tensor(const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda)
    {
        // return a {form} that, when contracted with {x} and {y}...
        return mito::manifolds::form<2>(
            [a_tilda, b_tilda]<class X, class Y>(const X & x, const Y & y) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y);
            });
    }

    // the tensor product of three one-forms
    template <class F1, class F2, class F3>
    constexpr auto tensor(
        const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda,
        const one_form_t<F3> & c_tilda)
    {
        // return a {form} that, when contracted with {x}, {y} and {z}...
        return mito::manifolds::form<3>(
            [a_tilda, b_tilda, c_tilda]<class X, class Y, class Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y) * c_tilda(z);
            });
    }
}


#endif

// end of file
