// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// Derivation rules


namespace mito::functions {

    // the {I...}-th first partial derivative of a function sum
    template <int... I, class F1, class F2>
    constexpr auto derivative(const Sum<F1, F2> & f)
    {
        return derivative<I...>(f.f1()) + derivative<I...>(f.f2());
    }

    // the {I...}-th first partial derivative of a constant plus a function
    template <int... I, tensor::tensor_or_scalar_c T, class F>
    constexpr auto derivative(const FunctionPlusConstant<T, F> & f)
    {
        return derivative<I...>(f.f());
    }

    // the {I...}-th first partial derivative of a function times a constant
    template <int... I, tensor::tensor_or_scalar_c T, class F>
    constexpr auto derivative(const FunctionTimesConstant<T, F> & f)
    {
        return derivative<I...>(f.f()) * f.constant();
    }

    // the {I...}-th first partial derivative of a constant times a function
    template <int... I, tensor::tensor_or_scalar_c T, class F>
    constexpr auto derivative(const ConstantTimesFunction<T, F> & f)
    {
        return f.constant() * derivative<I...>(f.f());
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

    // the derivative of the transpose of a function
    template <int... I, class F>
    constexpr auto derivative(const Transpose<F> & f)
    {
        // the derivative of the transpose is the transpose of the derivative
        return functions::transpose(derivative<I...>(f.f()));
    }

    // the {I...}-th first partial derivative of a linear combination
    template <int... I, typename T, function_c... Funcs>
    constexpr auto derivative(const LinearCombination<T, Funcs...> & f)
    {
        // the derivative of a linear combination is the linear combination of the derivatives
        return linear_combination(
            f.coefficients(), derivative<I...>(std::get<Funcs>(f.functions()))...);
    }
}


// end of file
