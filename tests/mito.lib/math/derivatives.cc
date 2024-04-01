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
    // the function pointer type
    using function_pointer_type = Y (*)(X);

  public:
    // constructor
    constexpr Function(F f) : _f{ f } {}

    // call operator
    constexpr auto operator()(X x) const -> Y { return _f(x); }

    // call operator for function composition
    template <class G>
    constexpr auto operator()(const Function<G> & g) const -> auto
    requires(std::convertible_to<typename Function<G>::Y, X>)
    {
        auto f = _f;
        return function([f, g](Function<G>::X x) -> Y { return f(g(x)); });
    }

    // cast to point to function
    operator function_pointer_type() const { return _f; };

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


namespace mito::math {

    constexpr auto cos =
        ::function([](const mito::vector_t<1> & x) -> mito::scalar_t { return std::cos(x); });
    constexpr auto x_0 =
        ::function([](const mito::vector_t<2> & x) -> mito::scalar_t { return x[0]; });
    constexpr auto x_1 =
        ::function([](const mito::vector_t<2> & x) -> mito::scalar_t { return x[1]; });
}


int
main()
{

    constexpr auto f = mito::math::cos(mito::math::x_0 * mito::math::x_1);

    std::cout << f({ 0.0, 1.0 }) << std::endl;

    // all done
    return 0;
}