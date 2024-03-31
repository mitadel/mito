// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

#include <gtest/gtest.h>
#include <mito/base.h>
#include <mito/math.h>


// forward declaration
template <class F>
class Function;


// functions factory
template <class F>
constexpr auto
function(F && f)
{
    return Function<F>(std::forward<F>(f));
}


template <class F>
class Function {

    // type traits on function type
    using traits = mito::math::lambda_traits<F>;

  public:
    // the input type
    using X = typename traits::argument_type;
    // the output type
    using Y = typename traits::result_type;

  public:
    // constructor
    constexpr Function(F f) : _f{ f } {}

    // call operator
    constexpr auto operator()(X x) const -> Y { return _f(x); }

    // call operator
    template <class G>
    constexpr auto operator()(const Function<G> & g) const -> auto
    requires(std::is_same_v<typename Function<G>::Y, X>)
    {
        auto f = _f;
        return function([f, g](Function<G>::X x) -> Y { return f(g(x)); });
    }

  private:
    // the function wrapped
    F _f;
};


// fa * fb
template <class F1, class F2>
constexpr auto
operator*(const Function<F1> & fA, const Function<F2> & fB)
requires(std::is_same_v<typename Function<F1>::X, typename Function<F2>::X>)
{
    return function([fA, fB](const Function<F1>::X & x) { return fA(x) * fB(x); });
}


// using mito::math;

int
main()
{
    constexpr auto cos = function([](mito::scalar_t x) { return std::cos(x); });
    constexpr auto x_0 = function([](mito::vector_t<2> x) -> mito::scalar_t { return { x[0] }; });
    // auto x_1 = function([](const mito::vector_t<2> & x) { return x[1]; });

    constexpr auto f = cos(x_0);

    std::cout << f(mito::vector_t<2>{ 0.0, 1.0 }) << std::endl;

    // all done
    return 0;
}