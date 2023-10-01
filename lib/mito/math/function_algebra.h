// code guard
#if !defined(mito_math_function_algebra_h)
#define mito_math_function_algebra_h


// Algebraic operations on Function
// TODO: see if expression templates can be used to avoid copies with intermediate results


namespace mito::math {

    // fa + fb (identical return type for fa and fb)
    template <class F1, class F2>
    constexpr auto operator+(const function_t<F1> & fA, const function_t<F2> & fB)
    requires(std::is_same<typename function_t<F1>::X, typename function_t<F2>::X>::value)
    {
        return function([fA, fB](const function_t<F1>::X & x) { return fA(x) + fB(x); });
    }

    // fa * fb
    template <class F1, class F2>
    constexpr auto operator*(const function_t<F1> & fA, const function_t<F2> & fB)
    requires(std::is_same<typename function_t<F1>::X, typename function_t<F2>::X>::value)
    {
        return function([fA, fB](const function_t<F1>::X & x) { return fA(x) * fB(x); });
    }

    // y * f (inner product)
    template <class F>
    constexpr auto operator*(const typename function_t<F>::Y & y, const function_t<F> & f)
    requires(function_t<F>::Y::size != 1)
    {
        return function([y, f](const function_t<F>::X & x) { return y * f(x); });
    }

    // f * y (inner product)
    template <class F>
    constexpr auto operator*(const function_t<F> & f, const typename function_t<F>::Y & y)
    requires(function_t<F>::Y::size != 1)
    {
        return y * f;
    }

    // a * f
    template <class F>
    constexpr auto operator*(const real & a, const function_t<F> & f)
    {
        return function([a, f](const function_t<F>::X & x) { return a * f(x); });
    }

    // f * a
    template <class F>
    constexpr auto operator*(const function_t<F> & f, const real & a)
    {
        return a * f;
    }

    // scalar function times tensor
    template <class F, typename T, class packingT, int... I>
    constexpr auto operator*(
        const function_t<F> & f, const pyre::tensor::Tensor<T, packingT, I...> & A)
    {
        return function([f, A](const function_t<F>::X & x) { return f(x) * A; });
    }

    // f / a
    template <class F>
    constexpr auto operator/(const function_t<F> & f, const real & a)
    {
        return (1.0 / a) * f;
    }

    // -f
    template <class F>
    constexpr auto operator-(const function_t<F> & f)
    {
        return -1.0 * f;
    }

    // fa - fb
    template <class F1, class F2>
    constexpr auto operator-(const function_t<F1> & fA, const function_t<F2> & fB)
    {
        return fA + (-fB);
    }

    // Special algebraic functions for scalar functions
    // a / f
    template <class F>
    constexpr auto operator/(const real & a, const function_t<F> & f)
    requires(std::is_same<typename function_t<F>::Y, scalar_t>::value)
    {
        return function([a, f](const function_t<F>::X & x) { return a / f(x); });
    }

    // f1 / f2
    template <class F1, class F2>
    constexpr auto operator/(const function_t<F1> & fa, const function_t<F2> & fb)
    requires(
        std::is_same<typename function_t<F1>::X, typename function_t<F2>::X>::value
        && std::is_same<typename function_t<F2>::Y, scalar_t>::value)
    {
        return function([fa, fb](const function_t<F1>::X & x) { return fa(x) / fb(x); });
    }

    // a + f
    template <class F>
    constexpr auto operator+(const real & a, const function_t<F> & f)
    requires(std::is_same<typename function_t<F>::Y, scalar_t>::value)
    {
        return function([a, f](const function_t<F>::X & x) { return a + f(x); });
    }

    // f + a
    template <class F>
    constexpr auto operator+(const function_t<F> & f, const real & a)
    requires(std::is_same<typename function_t<F>::Y, scalar_t>::value)
    {
        return a + f;
    }

    // a - f
    template <class F>
    constexpr auto operator-(const real & a, const function_t<F> & f)
    requires(std::is_same<typename function_t<F>::Y, scalar_t>::value)
    {
        return a + (-f);
    }

    // f - a
    template <class F>
    constexpr auto operator-(const function_t<F> & f, const real & a)
    requires(std::is_same<typename function_t<F>::Y, scalar_t>::value)
    {
        return f + (-a);
    }
}


#endif

// end of file
