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
    {
        return form<P>([fA, fB]<vector_or_dummy_c... argsT>(argsT... args) {
            return fA(args...) + fB(args...);
        });
    }

    // scalar * form
    template <int P, class F>
    constexpr auto operator*(const real & a, const form_t<P, F> & f)
    {
        return form<P>(
            [a, f]<vector_or_dummy_c... argsT>(argsT... args) { return a * f(args...); });
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
            [a_tilda, b_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y>(
                const X & x, const Y & y) -> auto {
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
            [a_tilda, b_tilda,
             c_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y, vector_or_dummy_c Z>(
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
            [a_tilda, b_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y>(
                const X & x, const Y & y) -> auto {
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
            [a_tilda, b_tilda,
             c_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y, vector_or_dummy_c Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y) * c_tilda(z);
            });
    }
}


#endif

// end of file
