
// code guard
#if !defined(mito_manifolds_OneForm_h)
#define mito_manifolds_OneForm_h


namespace mito::manifolds {

    // empty class for dummy vector for contraction
    class dummy_vector {};

    // instance of dummy vector for contraction
    static constexpr dummy_vector _;

    // TOFIX: forms should cast to tensors
    template <class F>
    class OneForm {
      private:
        // the dimension of the embedding space
        static constexpr int D = input<F>::input_dim;

      public:
        // the dimension of the embedding space
        static constexpr int dim = D;
        // alias for my underlying function type
        using function_type = F;

      public:
        // constructor
        constexpr OneForm(F f) : _f { f } {}

        // contraction with a vector
        constexpr auto operator()(const mito::vector_t<D> & x) const -> auto { return _f(x); }

        // contraction with a dummy vector (do not perform contraction)
        constexpr auto operator()(const dummy_vector &) const -> auto { return *this; }

      private:
        // the action of the one-form
        F _f;
    };
}

#endif    // mito_manifolds_OneForm_h

// end of file
