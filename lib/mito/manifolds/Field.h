
// code guard
#if !defined(mito_manifolds_Field_h)
#define mito_manifolds_Field_h


namespace mito::manifolds {

    template <class F>
    // TOFIX: add requirements of taking a position in input and returning a {Tensor} instance
    class Field {
      private:
        static constexpr int D = input<F>::input_dim;
        using return_type = input<F>::output_type;

      public:
        static constexpr int dim = D;
        using function_type = F;
        static constexpr int rank = input<F>::output_rank;

      public:
        // constructor
        constexpr Field(F f) : _f { f } {}

        // QUESTION: should this act on a {coordinates_t} or a {point_t}?
        // the value of the field at {x}
        constexpr auto operator()(const mito::geometry::coordinates_t<D> & x) const -> return_type
        {
            return _f(x);
        }

      private:
        // the function assigning the value of the tensor field at each point
        F _f;
    };
}


#endif    // mito_manifolds_Field_h

// end of file
