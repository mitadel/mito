// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_math_function_algebra_h)
#define mito_math_function_algebra_h


// Algebraic operations on Function
// TODO: see if expression templates can be used to avoid copies with intermediate results


namespace mito::math {

    // fa + fb
    constexpr auto operator+(const Function auto & f1, const Function auto & f2)
    {
        return Sum<std::remove_cvref_t<decltype(f1)>, std::remove_cvref_t<decltype(f2)>>(f1, f2);
    }

    // -f
    constexpr auto operator-(const Function auto & f)
    {
        return Negative<std::remove_cvref_t<decltype(f)>>(f);
    }

// TOFIX
#if 0

    // fa * fb
    template <class F1, class F2>
    constexpr auto operator*(const function_t<F1> & fA, const function_t<F2> & fB)
    requires(std::convertible_to<typename function_t<F1>::X, typename function_t<F2>::X>)
    {
        return function([fA, fB](function_t<F1>::X x) { return fA(x) * fB(x); });
    }

    // y * f (inner product)
    template <class F>
    constexpr auto operator*(const typename function_t<F>::Y & y, const function_t<F> & f)
    requires(function_t<F>::Y::size != 1)
    {
        return function([y, f](function_t<F>::X x) { return y * f(x); });
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
        return function([a, f](function_t<F>::X x) { return a * f(x); });
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
        return function([f, A](function_t<F>::X x) { return f(x) * A; });
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
    requires(std::convertible_to<typename function_t<F>::Y, scalar_t>)
    {
        return function([a, f](function_t<F>::X x) { return a / f(x); });
    }

    // f1 / f2
    template <class F1, class F2>
    constexpr auto operator/(const function_t<F1> & fa, const function_t<F2> & fb)
    requires(
        std::convertible_to<typename function_t<F1>::X, typename function_t<F2>::X>
        && std::convertible_to<typename function_t<F2>::Y, scalar_t>)
    {
        return function([fa, fb](function_t<F1>::X x) { return fa(x) / fb(x); });
    }

    // a + f
    template <class F>
    constexpr auto operator+(const real & a, const function_t<F> & f)
    requires(std::convertible_to<typename function_t<F>::Y, scalar_t>)
    {
        return function([a, f](function_t<F>::X x) { return a + f(x); });
    }

    // f + a
    template <class F>
    constexpr auto operator+(const function_t<F> & f, const real & a)
    requires(std::convertible_to<typename function_t<F>::Y, scalar_t>)
    {
        return a + f;
    }

    // a - f
    template <class F>
    constexpr auto operator-(const real & a, const function_t<F> & f)
    requires(std::convertible_to<typename function_t<F>::Y, scalar_t>)
    {
        return a + (-f);
    }

    // f - a
    template <class F>
    constexpr auto operator-(const function_t<F> & f, const real & a)
    requires(std::convertible_to<typename function_t<F>::Y, scalar_t>)
    {
        return f + (-a);
    }

#endif
}


#endif

// end of file
