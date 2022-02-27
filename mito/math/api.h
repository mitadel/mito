// code guard
#if !defined(mito_math_api_h)
#define mito_math_api_h


namespace mito::math {

    // make function from pointer to function
    template <class X, class Y>
    constexpr auto function(Y(f)(const X &));

    // make function from lambda function
    template <class F>
    constexpr auto function(F && f);

    // make a vector-valued function from N scalar-valued functions
    template <class X, int N>
    constexpr auto function(const function_t<X, scalar_t> (&f_list)[N]);

    // factories for field from either a field_t or a function_t
    template <class X, class Y, template <class, class> class FUNCTION>
    constexpr auto field(FUNCTION<X, Y> f);

    template <int D, class Y, template <class, class> class FUNCTION>
    constexpr auto field(
        FUNCTION<vector_t<D>, Y> f, std::array<FUNCTION<vector_t<D>, Y>, D> df);

    // factory for scalar field from either a scalar_field_t or a function_t
    template <int D, template <class, class> class FUNCTION>
    constexpr auto scalar_field(FUNCTION<vector_t<D>, scalar_t> f);

    template <int D, template <class, class> class FUNCTION>
    constexpr auto scalar_field(
        FUNCTION<vector_t<D>, scalar_t> f, std::array<FUNCTION<vector_t<D>, scalar_t>, D> df);

    template <int D, template <class, class> class FUNCTION>
    constexpr auto scalar_field(
        FUNCTION<vector_t<D>, scalar_t> f, FUNCTION<vector_t<D>, vector_t<D>> df);

    // factory for vector field from either a vector_field_t or a function_t
    template <int D, int N, template <class, class> class FUNCTION>
    constexpr auto vector_field(FUNCTION<vector_t<D>, vector_t<N>> f);

    template <int D, int N, template <class, class> class FUNCTION>
    constexpr auto vector_field(
        FUNCTION<vector_t<D>, vector_t<N>> f, std::array<FUNCTION<vector_t<D>, vector_t<N>>, D> df);

}


#endif

// end of file
