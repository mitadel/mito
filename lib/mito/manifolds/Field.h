
// code guard
#if !defined(mito_manifolds_Field_h)
#define mito_manifolds_Field_h


namespace mito::manifolds {

    template <class F>
    class Field {
      private:
        static constexpr int D = input<F>::input_dim;

      public:
        static constexpr int dim = D;
        using function_type = F;

      public:
        // constructor
        constexpr Field(F f) : _f { f } {}

        // QUESTION: should this act on a {coordinates_t} or a {point_t}?
        // the value of the field at position {x}
        constexpr auto operator()(const mito::geometry::coordinates_t<D> & x) const
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
