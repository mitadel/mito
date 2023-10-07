// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

    // addition of forms fa + fb
    template <class F1, class F2, int D>
    constexpr auto operator+(const one_form_t<F1, D> & fA, const one_form_t<F2, D> & fB)
    {
        return one_form([fA, fB](const mito::vector_t<D> & x) { return fA(x) + fB(x); });
    }

    // tensor product of forms
    template <class F1, class F2, int D>
    constexpr auto operator*(const one_form_t<F1, D> & fA, const one_form_t<F2, D> & fB)
    {
        return function([fA, fB](const mito::vector_t<D> & x) { return fA(x) * fB; });
    }

    // scalar * form
    template <class F, int D>
    constexpr auto operator*(const real & a, const one_form_t<F, D> & f)
    {
        return one_form([a, f](const mito::vector_t<D> & x) { return a * f(x); });
    }

    // form * scalar
    template <class F, int D>
    constexpr auto operator*(const one_form_t<F, D> & f, const real & a)
    {
        return a * f;
    }

    // unary operator- for forms
    template <class F, int D>
    constexpr auto operator-(const one_form_t<F, D> & f)
    {
        return -1.0 * f;
    }

    // subtraction of forms fa - fb
    template <class F1, class F2, int D>
    constexpr auto operator-(const one_form_t<F1, D> & fA, const one_form_t<F2, D> & fB)
    {
        return fA + (-fB);
    }

    // the wedge product of two one-forms
    template <class F1, class F2, int D = input<F1>::dim>
    constexpr auto wedge(const one_form_t<F1, D> & a_tilda, const one_form_t<F2, D> & b_tilda)
    {
        // TOFIX: this should return a two-form
        // return a two-form that, when contracted with {x}...
        return mito::math::function([a_tilda, b_tilda](const mito::vector_t<D> & x) -> auto {
            // ... returns the one-form prescribed by the wedge product
            return a_tilda(x) * b_tilda - b_tilda(x) * a_tilda;
        });
    }
}


#endif

// end of file
