
// code guard
#if !defined(mito_manifolds_OneForm_h)
#define mito_manifolds_OneForm_h


namespace mito::manifolds {

    template <class F>
    class Form {
      private:
        static constexpr int D = input<F>::input_dim;

      public:
        static constexpr int dim = D;
        using function_type = F;

      public:
        // constructor
        constexpr Form(F f) : _f { f } {}

        // contraction with a vector
        constexpr auto operator()(const mito::vector_t<D> & x) const -> auto { return _f(x); }

      private:
        // the action of the form
        F _f;
    };
}

#endif    // mito_math_Function_h

// end of file
