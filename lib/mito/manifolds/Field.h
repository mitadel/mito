
// code guard
#if !defined(mito_manifolds_Field_h)
#define mito_manifolds_Field_h


namespace mito::manifolds {

    template <class F>
    class Field {
      private:
        static constexpr int D = input<F>::input_dim;
        using output_type = input<F>::output_type;

      public:
        static constexpr int dim = D;
        using function_type = F;

      public:
        // constructor
        constexpr Field(F f) : _f { f } {}

        // the value of the field at position {x}
        constexpr auto operator()(const geometry::coordinates_t<D> & x) const -> output_type
        {
            return _f(x);
        }

      private:
        // the function assigning the value of the field at each point
        F _f;
    };
}


#endif    // mito_manifolds_Field_h

// end of file
