// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::tensor {

    // addition of forms fa + fb
    template <int P, class F1, class F2>
    constexpr auto operator+(const form_t<P, F1> & fA, const form_t<P, F2> & fB)
    {
        return form<P>([fA, fB]<vector_or_dummy_c... argsT>(argsT... args) {
            return fA(args...) + fB(args...);
        });
    }

    // product of forms fa * fb (returns the tensor product of one-forms)
    template <class F1, class F2>
    constexpr auto operator*(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    {
        return tensor(fA, fB);
    }

    // scalar * form
    template <int P, class F>
    constexpr auto operator*(const real & a, const form_t<P, F> & f)
    {
        return form<P>(
            [a, f]<vector_or_dummy_c... argsT>(argsT... args) { return a * f(args...); });
    }

    // form * scalar
    constexpr auto operator*(const p_form_c auto & f, const real & a)
    {
        return a * f;
    }

    // unary operator- for forms
    constexpr auto operator-(const p_form_c auto & f)
    {
        return -1.0 * f;
    }

    // subtraction of forms fa - fb
    constexpr auto operator-(const p_form_c auto & fA, const p_form_c auto & fB)
    {
        return fA + (-fB);
    }
}


#endif

// end of file
