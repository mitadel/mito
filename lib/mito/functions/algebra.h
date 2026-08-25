// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// Algebraic operations on functions


namespace mito::functions {

    // f1 + f2
    constexpr auto operator+(const function_c auto & f1, const function_c auto & f2)
    {
        return Summation(f1, f2);
    }

    // a + f
    constexpr auto operator+(const tensor::tensor_or_scalar_c auto & a, const function_c auto & f)
    {
        return FunctionPlusConstant(f, a);
    }

    // f + a
    constexpr auto operator+(const function_c auto & f, const tensor::tensor_or_scalar_c auto & a)
    {
        return a + f;
    }

    // a * f
    constexpr auto operator*(const tensor::tensor_or_scalar_c auto & a, const function_c auto & f)
    {
        return ConstantTimesFunction(a, f);
    }

    // f * a
    constexpr auto operator*(const function_c auto & f, const tensor::tensor_or_scalar_c auto & a)
    {
        return FunctionTimesConstant(f, a);
    }

    // f / a
    constexpr auto operator/(const function_c auto & f, const tensor::scalar_t & a)
    {
        return (1.0 / a) * f;
    }

    // -f
    constexpr auto operator-(const function_c auto & f)
    {
        return -1.0 * f;
    }

    // f1 - f2
    constexpr auto operator-(const function_c auto & f1, const function_c auto & f2)
    {
        return f1 + (-f2);
    }

    // a - f
    constexpr auto operator-(const tensor::tensor_or_scalar_c auto & a, const function_c auto & f)
    {
        return a + (-f);
    }

    // f - a
    constexpr auto operator-(const function_c auto & f, const tensor::tensor_or_scalar_c auto & a)
    {
        return f + (-a);
    }

    // f1 * f2
    constexpr auto operator*(const function_c auto & f1, const function_c auto & f2)
    {
        return Product(f1, f2);
    }

    // a / f
    constexpr auto operator/(
        const tensor::tensor_or_scalar_c auto & a, const scalar_valued_function_c auto & f)
    {
        return a * Reciprocal(f);
    }

    // f1 / f2
    constexpr auto operator/(const function_c auto & f1, const scalar_valued_function_c auto & f2)
    {
        return f1 * Reciprocal(f2);
    }

    // f transposed
    template <function_c F>
    constexpr auto transpose(const F & f)
    {
        // if the function is scalar valued
        if constexpr (tensor::scalar_c<typename F::output_type>) {
            // return f
            return f;
        } else {
            // return the transpose
            return Transpose(f);
        }
    }

    // determinant of f
    template <function_c F>
    constexpr auto determinant(const F & f)
    {
        // if the function is scalar valued
        if constexpr (tensor::scalar_c<typename F::output_type>) {
            // return f
            return f;
        } else {
            // return the determinant
            return Determinant(f);
        }
    }

    // f inverse
    template <function_c F>
    constexpr auto inverse(const F & f)
    {
        // if the function is scalar valued
        if constexpr (tensor::scalar_c<typename F::output_type>) {
            // return 1 / f
            return Reciprocal(f);
        } else {
            // return the inverse
            return Inverse(f);
        }
    }

    // dyadic product of vector-valued functions f and g
    constexpr auto dyadic(
        const vector_valued_function_c auto & f, const vector_valued_function_c auto & g)
    {
        return DyadicProduct(f, g);
    }
}


// end of file
