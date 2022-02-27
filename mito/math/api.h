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

    template <class X, class Y, template <class, class> class FUNCTION, class... Args>
    constexpr auto field(FUNCTION<X, Y> && f, Args &&... args);

    template <class F, class... Args>
    constexpr auto field(F && f, Args &&... args);

    template <int D, template <class, class> class FUNCTION, class... Args>
    constexpr auto scalar_field(FUNCTION<vector_t<D>, scalar_t> && f, Args &&... args);

    template <class F, class... Args>
    constexpr auto scalar_field(F && f, Args &&... args);

    template <int D, int N, template <class, class> class FUNCTION, class... Args>
    constexpr auto vector_field(FUNCTION<vector_t<D>, vector_t<N>> && f, Args &&... args);

    template <class F, class... Args>
    constexpr auto vector_field(F && f, Args &&... args);

}


#endif

// end of file
