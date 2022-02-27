// code guard
#if !defined(mito_math_factories_h)
#define mito_math_factories_h


namespace mito::math {

    // factories for function
    // make function from pointer to function
    template <class X, class Y>
    constexpr auto function(Y(f)(const X &))
    {
        return function_t<X, Y>(f);
    }
    // make function from lambda function
    template <class F>
    constexpr auto function(F && f)
    {
        typedef lambda_traits<remove_reference_lambda<decltype(f)>> traits;
        using X = typename std::remove_reference<typename traits::argument_type>::type;
        using Y = typename std::remove_reference<typename traits::result_type>::type;
        return function_t<std::remove_const_t<X>, Y>(f);
    }
    // make a vector-valued function from N scalar-valued functions
    template <class X, int N>
    constexpr auto function(const function_t<X, scalar_t> (&f_list)[N])
    {
        return function_t<X, vector_t<N>>(f_list);
    }

    // factories for field from either a field_t or a function_t
    template <class X, class Y, template <class, class> class FUNCTION>
    constexpr auto field(FUNCTION<X, Y> f)
    {
        return field_t<X, Y>(f);
    }

    template <int D, class Y, template <class, class> class FUNCTION>
    constexpr auto field(
        FUNCTION<vector_t<D>, Y> f, std::array<FUNCTION<vector_t<D>, Y>, D> df)
    {
        return field_t<vector_t<D>, Y>(f, df);
    }

    // factory for scalar field from either a scalar_field_t or a function_t
    template <int D, template <class, class> class FUNCTION>
    constexpr auto scalar_field(FUNCTION<vector_t<D>, scalar_t> f)
    {
        return scalar_field_t<D>(f);
    }

    template <int D, template <class, class> class FUNCTION>
    constexpr auto scalar_field(
        FUNCTION<vector_t<D>, scalar_t> f, std::array<FUNCTION<vector_t<D>, scalar_t>, D> df)
    {
        return scalar_field_t<D>(f, df);
    }

    template <int D, template <class, class> class FUNCTION>
    constexpr auto scalar_field(
        FUNCTION<vector_t<D>, scalar_t> f, FUNCTION<vector_t<D>, vector_t<D>> df)
    {
        return scalar_field_t<D>(f, df);
    }

    // factory for vector field from either a vector_field_t or a function_t
    template <int D, int N, template <class, class> class FUNCTION>
    constexpr auto vector_field(FUNCTION<vector_t<D>, vector_t<N>> f)
    {
        return vector_field_t<D, N>(f);
    }

    template <int D, int N, template <class, class> class FUNCTION>
    constexpr auto vector_field(
        FUNCTION<vector_t<D>, vector_t<N>> f, std::array<FUNCTION<vector_t<D>, vector_t<N>>, D> df)
    {
        return vector_field_t<D, N>(f, df);
    }

}


#endif

// end of file
