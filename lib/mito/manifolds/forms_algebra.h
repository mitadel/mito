// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

    // addition of forms fa + fb
    template <class F1, class F2>
    constexpr auto operator+(const form_t<F1> & fA, const form_t<F2> & fB)
    requires(form_t<F1>::dim == form_t<F2>::dim)
    {
        // the dimension of the vector space
        constexpr int D = form_t<F1>::dim;
        return form([fA, fB](const mito::vector_t<D> & x) { return fA(x) + fB(x); });
    }

    // scalar * form
    template <class F>
    constexpr auto operator*(const real & a, const form_t<F> & f)
    {
        // the dimension of the vector space
        constexpr int D = form_t<F>::dim;
        return form([a, f](const mito::vector_t<D> & x) { return a * f(x); });
    }

    // form * scalar
    template <class F>
    constexpr auto operator*(const form_t<F> & f, const real & a)
    {
        return a * f;
    }

    // tensor product of forms
    template <class F1, class F2>
    constexpr auto operator*(const form_t<F1> & fA, const form_t<F2> & fB)
    {
        // the dimension of the vector space
        constexpr int D = form_t<F1>::dim;
        // NOTE: why fA(x) * fB and not fA * fB(x)? be aware that we are introducing a convention
        // here
        return form([fA, fB](const mito::vector_t<D> & x) { return fA(x) * fB; });
    }

    // unary operator- for forms
    template <class F>
    constexpr auto operator-(const form_t<F> & f)
    {
        return -1.0 * f;
    }

    // subtraction of forms fa - fb
    template <class F1, class F2>
    constexpr auto operator-(const form_t<F1> & fA, const form_t<F2> & fB)
    {
        return fA + (-fB);
    }

    // the wedge product of two one-forms
    template <class F1, class F2>
    constexpr auto wedge(const form_t<F1> & a_tilda, const form_t<F2> & b_tilda)
    requires(
        // {a_tilda} and {b_tilda} are one-forms...
        is_one_form<form_t<F1>>
        && is_one_form<form_t<F2>>
        // ... defined on the same vector space
        && form_t<F1>::dim == form_t<F2>::dim)
    {
        // the dimension of the vector space
        constexpr int D = form_t<F1>::dim;
        // return a form that, when contracted with {x}...
        return form([a_tilda, b_tilda](const mito::vector_t<D> & x) -> auto {
            // ... returns the form prescribed by the wedge product
            return (a_tilda * b_tilda - b_tilda * a_tilda)(x);
        });
    }

    // the wedge product of three one-forms
    template <class F1, class F2, class F3>
    constexpr auto wedge(
        const form_t<F1> & a_tilda, const form_t<F2> & b_tilda, const form_t<F3> & c_tilda)
    requires(
        // {a_tilda}, {b_tilda} and {c_tilda} are one-forms...
        is_one_form<form_t<F1>> && is_one_form<form_t<F2>>
        && is_one_form<form_t<F3>>
        // defined on the same vector space
        && form_t<F1>::dim == form_t<F2>::dim && form_t<F1>::dim == form_t<F3>::dim)
    {
        // the dimension of the vector space
        constexpr int D = form_t<F1>::dim;
        // return a form that, when contracted with {x}...
        return form([a_tilda, b_tilda, c_tilda](const mito::vector_t<D> & x) -> auto {
            // ... returns the form prescribed by the wedge product
            return a_tilda(x) * b_tilda * c_tilda - a_tilda(x) * c_tilda * b_tilda
                 + b_tilda(x) * c_tilda * a_tilda - b_tilda(x) * a_tilda * c_tilda
                 + c_tilda(x) * a_tilda * b_tilda - c_tilda(x) * b_tilda * a_tilda;
        });
    }
}


#endif

// end of file
