// code guard
#if !defined(mito_math_factories_h)
#define mito_math_factories_h


namespace mito::math {

    // factories for function
    template <class X, class Y, template <class, class> class FUNCTION>
    constexpr auto function(FUNCTION<X, Y> && f)
    {
        return function_t<X, Y>(std::forward<FUNCTION<X, Y>>(f));
    }
    template <class F>
    constexpr auto function(F && f)
    {
        typedef lambda_traits<remove_reference_lambda<decltype(f)>> traits;
        using X = typename std::remove_reference<typename traits::argument_type>::type;
        using Y = typename std::remove_reference<typename traits::result_type>::type;
        return function_t<X, Y>(f);
    }

    // factories for field
    template <class X, class Y, template <class, class> class FUNCTION, class... Args>
    constexpr auto field(FUNCTION<X, Y> && f, Args &&... args)
    {
        return field_t<X, Y>(std::forward<FUNCTION<X, Y>>(f), std::forward<Args>(args)...);
    }
    template <class F, class... Args>
    constexpr auto field(F && f, Args &&... args)
    {
        typedef lambda_traits<remove_reference_lambda<decltype(f)>> traits;
        using X = typename std::remove_reference<typename traits::argument_type>::type;
        using Y = typename std::remove_reference<typename traits::result_type>::type;
        return field_t<X, Y>(std::forward<F>(f), std::forward<Args>(args)...);
    }

    // factories for scalar field
    template <int D, template <class, class> class FUNCTION, class... Args>
    constexpr auto scalar_field(FUNCTION<vector_t<D>, scalar_t> && f, Args &&... args)
    {
        return scalar_field_t<D>(
            std::forward<FUNCTION<vector_t<D>, scalar_t>>(f), std::forward<Args>(args)...);
    }
    template <class F, class... Args>
    constexpr auto scalar_field(F && f, Args &&... args)
    {
        typedef lambda_traits<remove_reference_lambda<decltype(f)>> traits;
        using X = typename std::remove_reference<typename traits::argument_type>::type;
        constexpr int D = X::size;
        return scalar_field_t<D>(std::forward<F>(f), std::forward<Args>(args)...);
    }

    // factories for vector field
    template <int D, int N, template <class, class> class FUNCTION, class... Args>
    constexpr auto vector_field(FUNCTION<vector_t<D>, vector_t<N>> && f, Args &&... args)
    {
        return vector_field_t<D, N>(
            std::forward<FUNCTION<vector_t<D>, vector_t<N>>>(f), std::forward<Args>(args)...);
    }
    template <class F, class... Args>
    constexpr auto vector_field(F && f, Args &&... args)
    {
        typedef lambda_traits<remove_reference_lambda<decltype(f)>> traits;
        using X = typename std::remove_reference<typename traits::argument_type>::type;
        constexpr int D = X::size;
        using Y = typename std::remove_reference<typename traits::result_type>::type;
        constexpr int N = Y::size;
        return vector_field_t<D, N>(std::forward<F>(f), std::forward<Args>(args)...);
    }

}


#endif

// end of file
