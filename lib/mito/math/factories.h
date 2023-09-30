// code guard
#if !defined(mito_math_factories_h)
#define mito_math_factories_h


namespace mito::math {

    // factory returning a constant function
    template <class X, real c>
    constexpr auto constant()
    {
        return function([](const X &) { return c; });
    }

    // factory returning the function {f(x) = 1.0}
    template <class X>
    constexpr auto one()
    {
        return constant<X, 1.0>();
    }

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
        using X = typename std::remove_reference_t<typename traits::argument_type>;
        using Y = typename std::remove_reference_t<typename traits::result_type>;
        return function_t<std::remove_const_t<X>, Y>(f);
    }
    // make a vector-valued function from N scalar-valued functions
    template <class X, int N>
    constexpr auto function(const function_t<X, scalar_t> (&f_list)[N])
    {
        return function_t<X, vector_t<N>>(f_list);
    }

    // factories for field from either a field_t or a function_t (without derivatives)
    template <class X, class Y, template <class, class> class FUNCTION>
    constexpr auto field(FUNCTION<X, Y> f)
    {
        return field_t<X, Y>(f);
    }

    // factories for field from either a field_t or a function_t (with derivatives in an array)
    template <int D, class Y, template <class, class> class FUNCTION>
    constexpr auto field(FUNCTION<vector_t<D>, Y> f, std::array<FUNCTION<vector_t<D>, Y>, D> df)
    {
        return field_t<vector_t<D>, Y>(f, df);
    }

    // construct a one-form based on its metric-equivalent vector
    template <int D>
    constexpr auto one_form(mito::vector_t<D> vector)
    {
        // return the contraction of a given vector {x} with {vector}
        return [vector](const mito::vector_t<D> & x) -> mito::scalar_t {
            // return the contraction of {vector} with {x}
            return vector * x;
        };
    }
}


#endif

// end of file
