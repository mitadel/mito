// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

    // addition of forms fa + fb
    template <class F1, class F2>
    constexpr auto operator+(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    requires(one_form_t<F1>::dim == one_form_t<F2>::dim)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F1>::dim;
        return one_form([fA, fB](const mito::vector_t<D> & x) { return fA(x) + fB(x); });
    }

    // scalar * form
    template <class F>
    constexpr auto operator*(const real & a, const one_form_t<F> & f)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F>::dim;
        return one_form([a, f](const mito::vector_t<D> & x) { return a * f(x); });
    }

    // form * scalar
    template <class F>
    constexpr auto operator*(const one_form_t<F> & f, const real & a)
    {
        return a * f;
    }

    // tensor product of forms
    template <class F1, class F2>
    constexpr auto operator*(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F1>::dim;
        // NOTE: why fA(x) * fB and not fA * fB(x)? be aware that we are introducing a convention
        // here
        return one_form([fA, fB](const mito::vector_t<D> & x) { return fA(x) * fB; });
    }

    // unary operator- for forms
    template <class F>
    constexpr auto operator-(const one_form_t<F> & f)
    {
        return -1.0 * f;
    }

    // subtraction of forms fa - fb
    template <class F1, class F2>
    constexpr auto operator-(const one_form_t<F1> & fA, const one_form_t<F2> & fB)
    {
        return fA + (-fB);
    }

    // scalar field * form (this is legal because scalar fields are zero-forms)
    template <class F1, class F2>
    constexpr auto operator*(const field_t<F1> & a, const one_form_t<F2> & f)
        // require that {a} is a scalar field
    requires(field_t<F1>::rank == 1)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F2>::dim;
        return field([a, f](const mito::vector_t<D> & x) { return a(x) * f(x); });
    }

    // form * scalar field (this is legal because scalar fields are zero-forms)
    template <class F1, class F2>
    constexpr auto operator*(const one_form_t<F1> & f, const field_t<F2> & a)
        // require that {a} is a scalar field
    requires(field_t<F2>::rank == 1)
    {
        return a * f;
    }

    // the wedge product of two one-forms
    template <class F1, class F2>
    constexpr auto wedge(const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda)
    requires(
        // {a_tilda} and {b_tilda} are defined on the same vector space
        one_form_t<F1>::dim == one_form_t<F2>::dim)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F1>::dim;
        // return a one-form that, when contracted with {x}...
        return one_form([a_tilda, b_tilda](const mito::vector_t<D> & x) -> auto {
            // ... returns the form prescribed by the wedge product
            return a_tilda(x) * b_tilda - b_tilda(x) * a_tilda;
        });
    }

    // the wedge product of three one-forms
    template <class F1, class F2, class F3>
    constexpr auto wedge(
        const one_form_t<F1> & a_tilda, const one_form_t<F2> & b_tilda,
        const one_form_t<F3> & c_tilda)
    requires(
        // {a_tilda}, {b_tilda} and {c_tilda} are defined on the same vector space
        one_form_t<F1>::dim == one_form_t<F2>::dim && one_form_t<F1>::dim == one_form_t<F3>::dim)
    {
        // the dimension of the vector space
        constexpr int D = one_form_t<F1>::dim;
        // return a form that, when contracted with {x}...
        return one_form([a_tilda, b_tilda, c_tilda](const mito::vector_t<D> & x) -> auto {
            // ... returns the form prescribed by the wedge product
            return a_tilda(x) * wedge(b_tilda, c_tilda) + b_tilda(x) * wedge(c_tilda, a_tilda)
                 + c_tilda(x) * wedge(a_tilda, b_tilda);
        });
    }
}


#endif

// end of file
