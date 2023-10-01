// code guard
#if !defined(mito_math_forward_h)
#define mito_math_forward_h


namespace mito::math {

    // class function
    template <class F>
    class Function;

    // alias for function
    template <class F>
    using function_t = Function<F>;

    // class field
    template <class X, class Y = scalar_t>
    class Field;

    // alias for field
    template <class X, class Y>
    using field_t = Field<X, Y>;

    // scalar field
    template <int D>
    using ScalarField = Field<vector_t<D>, scalar_t>;

    // alias for scalar field
    template <int D>
    using scalar_field_t = ScalarField<D>;

    // vector field
    template <int D, int N>
    using VectorField = Field<vector_t<D>, vector_t<N>>;

    // alias for vector field
    template <int D, int N>
    using vector_field_t = VectorField<D, N>;

}


#endif

// end of file
