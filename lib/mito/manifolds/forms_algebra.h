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
        // return a {form} that, when contracted with {x} and {y}...
        return mito::manifolds::form<2>(
            [a_tilda, b_tilda]<class X, class Y>(const X & x, const Y & y) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * b_tilda(y) - b_tilda(x) * a_tilda(y);
            });
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
