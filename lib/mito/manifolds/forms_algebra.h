// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

    // addition of forms fa + fb
    template <class F1, class F2, int D>
    constexpr auto operator+(const form_t<F1, D> & fA, const form_t<F2, D> & fB)
    {
        return form([fA, fB](const mito::vector_t<D> & x) { return fA(x) + fB(x); });
    }

    // scalar * form
    template <class F, int D>
    constexpr auto operator*(const real & a, const form_t<F, D> & f)
    {
        return form([a, f](const mito::vector_t<D> & x) { return a * f(x); });
    }

    // form * scalar
    template <class F, int D>
    constexpr auto operator*(const form_t<F, D> & f, const real & a)
    {
        return a * f;
    }

    // tensor product of forms
    template <class F1, class F2, int D>
    constexpr auto operator*(const form_t<F1, D> & fA, const form_t<F2, D> & fB)
    {
        return form([fA, fB](const mito::vector_t<D> & x) { return fA(x) * fB; });
    }

    // unary operator- for forms
    template <class F, int D>
    constexpr auto operator-(const form_t<F, D> & f)
    {
        return -1.0 * f;
    }

    // subtraction of forms fa - fb
    template <class F1, class F2, int D>
    constexpr auto operator-(const form_t<F1, D> & fA, const form_t<F2, D> & fB)
    {
        return fA + (-fB);
    }

    // the wedge product of two one-forms
    template <class F1, class F2, int D = input<F1>::dim>
    constexpr auto wedge(const form_t<F1, D> & a_tilda, const form_t<F2, D> & b_tilda)
    requires(is_one_form<form_t<F1, D>> && is_one_form<form_t<F2, D>>)
    {
        // return a form that, when contracted with {x}...
        return form([a_tilda, b_tilda](const mito::vector_t<D> & x) -> auto {
            // ... returns the form prescribed by the wedge product
            return (a_tilda * b_tilda - b_tilda * a_tilda)(x);
        });
    }

        });
    }
}


#endif

// end of file
