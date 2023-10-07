
// code guard
#if !defined(mito_manifolds_OneForm_h)
#define mito_manifolds_OneForm_h


namespace mito::manifolds {

    template <class F, int D = input<F>::dim>
    requires(is_one_form<mito::math::function_t<F>, D>)
    class OneForm {
      public:
        // constructor
        constexpr OneForm(F f) : _f { f } {}

        // call operator
        constexpr auto operator()(const mito::vector_t<D> x) const -> mito::scalar_t
        {
            return _f(x);
        }

      private:
        // the action of the form
        F _f;
    };
}

#endif    // mito_math_Function_h

// end of file
