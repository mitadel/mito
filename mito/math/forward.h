// code guard
#if !defined(mito_math_forward_h)
#define mito_math_forward_h


namespace mito::math {

    // alias for functor
    template <class X, class Y = scalar_t>
    using functor_t = std::function<Y(const X &)>;

    // class function
    template <class X, class Y = scalar_t>
    class Function;

    // alias for function
    template <class X, class Y = scalar_t>
    using function_t = Function<X, Y>;

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
