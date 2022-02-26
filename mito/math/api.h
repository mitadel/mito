// code guard
#if !defined(mito_math_api_h)
#define mito_math_api_h


namespace mito::math {

    template <class X, class Y>
    constexpr auto function(Y(f)(const X &));

    template <class F>
    constexpr auto function(F && f);

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
