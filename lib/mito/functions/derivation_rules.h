// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_derivation_rules_h)
#define mito_functions_derivation_rules_h


// Derivation rules


namespace mito::functions {

    // the first derivative of a base function
    constexpr auto derivative(const function_c auto & f)
    {
        return _derivative<std::remove_cvref_t<decltype(f)>>();
    }

    // the first derivative of a function sum
    template <class F1, class F2>
    constexpr auto derivative(const Sum<F1, F2> & f)
    {
        return derivative(f.f1()) + derivative(f.f2());
    }

    // the first derivative of a constant plus a function
    template <class F>
    constexpr auto derivative(const FunctionPlusConstant<F> & f)
    {
        return derivative(f.f());
    }

    // the first derivative of a constant times a function
    template <class F>
    constexpr auto derivative(const FunctionTimesConstant<F> & f)
    {
        return f.scalar() * derivative(f.f());
    }

    // the first derivative of a product
    template <class F1, class F2>
    constexpr auto derivative(const Product<F1, F2> & f)
    {
        return f.f1() * derivative(f.f2()) + derivative(f.f1()) * f.f2();
    }

    // the first derivative of a reciprocal
    template <class F>
    constexpr auto derivative(const Reciprocal<F> & f)
    {
        return -1.0 * derivative(f.f()) / (f.f() * f.f());
    }
}


#endif

// end of file
