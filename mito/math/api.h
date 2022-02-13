// code guard
#if !defined(mito_math_api_h)
#define mito_math_api_h


namespace mito::math {

    // template typedef for functor
    template <class X, class Y = scalar_t>
    using functor_t = std::function<Y(const X &)>;

    // template typedef for function
    template <class X, class Y = scalar_t>
    using function_t = Function<X, Y>;

    template <class X, class Y, class... Args>
    constexpr auto function(Args &&... args)
    {
        return function_t<X, Y>(std::forward<Args>(args)...);
    }

    // field
    template <class X, class Y>
    using field_t = Field<X, Y>;

    template <class X, class Y, class... Args>
    constexpr auto field(Args &&... args)
    {
        return field_t<X, Y>(std::forward<Args>(args)...);
    }

    // vector field
    template <int D, int N>
    using VectorField = Field<vector_t<D>, vector_t<N>>;

    template <int D, int N>
    using vector_field_t = VectorField<D, N>;

    template <int D, int N, class... Args>
    constexpr auto vector_field(Args &&... args)
    {
        return vector_field_t<D, N>(std::forward<Args>(args)...);
    }

    // scalar field
    template <int D>
    using ScalarField = Field<vector_t<D>, scalar_t>;

    template <int D>
    using scalar_field_t = ScalarField<D>;

    template <int D, class... Args>
    constexpr auto scalar_field(Args &&... args)
    {
        return scalar_field_t<D>(std::forward<Args>(args)...);
    }
}


#endif

// end of file
