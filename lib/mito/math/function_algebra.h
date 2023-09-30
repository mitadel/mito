// code guard
#if !defined(mito_math_function_algebra_h)
#define mito_math_function_algebra_h


// Algebraic operations on Function
// TODO: see if expression templates can be used to avoid copies with intermediate results


namespace mito::math {

    // fa + fb (identical return type for fa and fb)
    template <class X, class Y>
    constexpr auto operator+(const function_t<X, Y> & fA, const function_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return function_t<X, Y>([fA, fB](const X & x) { return fA(x) + fB(x); });
    }

    // fa + fb (return type for fa is convertible to return type for fb)
    template <class X, class Y, class Z>
    constexpr auto operator+(const function_t<X, Y> & fA, const function_t<X, Z> & fB)
        -> function_t<X, Z>
    requires(std::is_convertible<Y, Z>::value)
    {
        return function_t<X, Z>([fA, fB](const X & x) { return fA(x) + fB(x); });
    }

    // fa + fb (return type for fb is convertible to return type for fa)
    template <class X, class Y, class Z>
    constexpr auto operator+(const function_t<X, Y> & fA, const function_t<X, Z> & fB)
        -> function_t<X, Y>
    requires(std::is_convertible<Z, Y>::value)
    {
        return function_t<X, Y>([fA, fB](const X & x) { return fA(x) + fB(x); });
    }

    // fa * fb
    template <class X, class Y>
    constexpr auto operator*(const function_t<X, Y> & fA, const function_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return function_t<X, Y>([fA, fB](const X & x) { return fA(x) * fB(x); });
    }

    // y * f (inner product)
    template <class X, class Y>
    constexpr auto operator*(const Y & y, const function_t<X, Y> & f)
        -> function_t<X, typename type<Y>::value>
    requires(Y::size != 1)
    {
        return function_t<X, typename type<Y>::value>([y, f](const X & x) { return y * f(x); });
    }

    // f * y (inner product)
    template <class X, class Y>
    constexpr auto operator*(const function_t<X, Y> & f, const Y & y)
        -> function_t<X, typename type<Y>::value>
    requires(Y::size != 1)
    {
        return y * f;
    }

    // a * f
    template <class X, class Y>
    constexpr auto operator*(const real & a, const function_t<X, Y> & f) -> function_t<X, Y>
    {
        return function_t<X, Y>([a, f](const X & x) { return a * f(x); });
    }

    // f * a
    template <class X, class Y>
    constexpr auto operator*(const function_t<X, Y> & f, const real & a) -> function_t<X, Y>
    {
        return a * f;
    }

    // scalar function times tensor
    template <class X, typename T, class packingT, int... I>
    constexpr auto operator*(
        const function_t<X, real> & f, const pyre::tensor::Tensor<T, packingT, I...> & A)
        -> function_t<X, pyre::tensor::Tensor<T, packingT, I...>>
    {
        return function_t<X, pyre::tensor::Tensor<T, packingT, I...>>(
            [f, A](const X & x) { return f(x) * A; });
    }

    // f / a
    template <class X, class Y>
    constexpr auto operator/(const function_t<X, Y> & f, const real & a) -> function_t<X, Y>
    {
        return (1.0 / a) * f;
    }

    // -f
    template <class X, class Y>
    constexpr auto operator-(const function_t<X, Y> & f) -> function_t<X, Y>
    {
        return -1.0 * f;
    }

    // fa - fb
    template <class X, class Y>
    constexpr auto operator-(const function_t<X, Y> & fA, const function_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return fA + (-fB);
    }

    // Special algebraic functions for scalar functions
    // a / f
    template <class X>
    constexpr auto operator/(const real & a, const function_t<X, scalar_t> & f)
        -> function_t<X, scalar_t>
    {
        return function_t<X, scalar_t>([a, f](const X & x) { return a / f(x); });
    }

    // f1 / f2
    template <class X, class Y>
    constexpr auto operator/(const function_t<X, Y> & f1, const function_t<X, scalar_t> & f2)
        -> function_t<X, Y>
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // a + f
    template <class X>
    constexpr auto operator+(const real & a, const function_t<X, scalar_t> & f)
        -> function_t<X, scalar_t>
    {
        return function_t<X, scalar_t>([a, f](const X & x) { return a + f(x); });
    }

    // f + a
    template <class X>
    constexpr auto operator+(const function_t<X, scalar_t> & f, const real & a)
        -> function_t<X, scalar_t>
    {
        return a + f;
    }

    // a - f
    template <class X>
    constexpr auto operator-(const real & a, const function_t<X, scalar_t> & f)
        -> function_t<X, scalar_t>
    {
        return a + (-f);
    }

    // f - a
    template <class X>
    constexpr auto operator-(const function_t<X, scalar_t> & f, const real & a)
        -> function_t<X, scalar_t>
    {
        return f + (-a);
    }

    // Binary operators for functor_t<X, Y> and function_t<X, Y>, such as:
    // fa + fb
    template <class X, class Y>
    constexpr auto operator+(const function_t<X, Y> & fA, const functor_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return fA + function_t<X, Y>(fB);
    }

    // fa + fb
    template <class X, class Y>
    constexpr auto operator+(const functor_t<X, Y> & fA, const function_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return fB + fA;
    }

    // fa * fb
    template <class X, class Y>
    constexpr auto operator*(const function_t<X, Y> & fA, const functor_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return fA * function_t<X, Y>(fB);
    }

    // fa * fb
    template <class X, class Y>
    constexpr auto operator*(const functor_t<X, Y> & fA, const function_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return fB * fA;
    }

    // fa - fb
    template <class X, class Y>
    constexpr auto operator-(const function_t<X, Y> & fA, const functor_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return fA - function_t<X, Y>(fB);
    }

    // fa - fb
    template <class X, class Y>
    constexpr auto operator-(const functor_t<X, Y> & fA, const function_t<X, Y> & fB)
        -> function_t<X, Y>
    {
        return -(fB - fA);
    }

    // f1 / f2
    template <class X, class Y>
    constexpr auto operator/(const function_t<X, Y> & f1, const functor_t<X, scalar_t> & f2)
        -> function_t<X, Y>
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // f1 / f2
    template <class X, class Y>
    constexpr auto operator/(const functor_t<X, Y> & f1, const function_t<X, scalar_t> & f2)
        -> function_t<X, Y>
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // Binary operators for Y(const X &) and function_t<X, Y>, such as:
    // fa + fb
    template <class X, class Y>
    constexpr auto operator+(const function_t<X, Y> & fA, Y fB(const X &)) -> function_t<X, Y>
    {
        return fA + function_t<X, Y>(fB);
    }

    // fa + fb
    template <class X, class Y>
    constexpr auto operator+(Y fA(const X &), const function_t<X, Y> & fB) -> function_t<X, Y>
    {
        return fB + fA;
    }

    // fa * fb
    template <class X, class Y>
    constexpr auto operator*(const function_t<X, Y> & fA, Y fB(const X &)) -> function_t<X, Y>
    {
        return fA * function_t<X, Y>(fB);
    }

    // fa * fb
    template <class X, class Y>
    constexpr auto operator*(Y fA(const X &), const function_t<X, Y> & fB) -> function_t<X, Y>
    {
        return fB * fA;
    }

    // fa - fb
    template <class X, class Y>
    constexpr auto operator-(const function_t<X, Y> & fA, Y fB(const X &)) -> function_t<X, Y>
    {
        return fA - function_t<X, Y>(fB);
    }

    // fa - fb
    template <class X, class Y>
    constexpr auto operator-(Y fA(const X &), const function_t<X, Y> & fB) -> function_t<X, Y>
    {
        return -(fB - fA);
    }

    // f1 / f2
    template <class X, class Y>
    constexpr auto operator/(const function_t<X, Y> & f1, real f2(const X &)) -> function_t<X, Y>
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }

    // f1 / f2
    template <class X, class Y>
    constexpr auto operator/(Y f1(const X &), const function_t<X, scalar_t> & f2)
        -> function_t<X, Y>
    {
        return function_t<X, Y>([f1, f2](const X & x) { return f1(x) / f2(x); });
    }
}


#endif

// end of file
