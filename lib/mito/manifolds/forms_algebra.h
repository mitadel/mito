// code guard
#if !defined(mito_manifold_forms_h)
#define mito_manifold_forms_h


namespace mito::manifolds {

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
