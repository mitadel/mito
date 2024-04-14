// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_derivation_rules_h)
#define mito_functions_derivation_rules_h


// Derivation rules


namespace mito::functions {

    // the {I...}-th first partial derivative of a function sum
    template <int... I, class F1, class F2>
    constexpr auto derivative(const Sum<F1, F2> & f)
    {
        return derivative<I...>(f.f1()) + derivative<I...>(f.f2());
    }

    // the {I...}-th first partial derivative of a constant plus a function
    template <int... I, class F>
    constexpr auto derivative(const FunctionPlusConstant<F> & f)
    {
        return derivative<I...>(f.f());
    }

    // the {I...}-th first partial derivative of a constant times a function
    template <int... I, class F>
    constexpr auto derivative(const FunctionTimesConstant<F> & f)
    {
        return f.scalar() * derivative<I...>(f.f());
    }

    // the {I...}-th first partial derivative of a product of functions
    template <int... I, class F1, class F2>
    constexpr auto derivative(const Product<F1, F2> & f)
    {
        return f.f1() * derivative<I...>(f.f2()) + derivative<I...>(f.f1()) * f.f2();
    }

    // the {I...}-th first partial derivative of a product of the reciprocal function
    template <int... I, class F>
    constexpr auto derivative(const Reciprocal<F> & f)
    {
        return -1.0 * derivative<I...>(f.f()) / (f.f() * f.f());
    }

    // the chain rule for the  {I...}-th first partial derivative
    template <int... I, class F1, class F2>
    constexpr auto derivative(const Composition<F1, F2> & f)
    {
        return derivative<I...>(f.f1())(f.f2()) * derivative<I...>(f.f2());
    }
}


#endif

// end of file
