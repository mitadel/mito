// code guard
#if !defined(mito_math_api_h)
#define mito_math_api_h


namespace mito::math {

    // make function from lambda function
    template <class F>
    constexpr auto function(F && f);

    // // make a vector-valued function from N scalar-valued functions
    // template <class X, int N>
    // constexpr auto function(const function_t<X, scalar_t> (&f_list)[N]);

    // factories for field from either a field_t or a function_t (without derivatives)
    template <class X, class Y, template <class, class> class FUNCTION>
    constexpr auto field(FUNCTION<X, Y> f);

    // factories for field from either a field_t or a function_t (with derivatives in an array)
    template <int D, class Y, template <class, class> class FUNCTION>
    constexpr auto field(FUNCTION<vector_t<D>, Y> f, std::array<FUNCTION<vector_t<D>, Y>, D> df);

    // factories for field from either a field_t or a function_t (with derivatives in a vector)
    // only for scalar fields (gradient is a vector)
    template <int D, template <class, class> class FUNCTION>
    constexpr auto field(FUNCTION<vector_t<D>, scalar_t> f, FUNCTION<vector_t<D>, vector_t<D>> df);

}


#endif

// end of file
