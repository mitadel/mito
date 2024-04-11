// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_algebra_h)
#define mito_functions_algebra_h


// Algebraic operations on functions


namespace mito::functions {

    // f1 + f2
    constexpr auto operator+(const function_c auto & f1, const function_c auto & f2)
    {
        return Sum(f1, f2);
    }

    // a + f
    constexpr auto operator+(const real & a, const function_c auto & f)
    {
        return FunctionPlusConstant(f, a);
    }

    // f + a
    constexpr auto operator+(const function_c auto & f, const real & a)
    {
        return a + f;
    }

    // a * f
    constexpr auto operator*(const real & a, const function_c auto & f)
    {
        return FunctionTimesConstant(f, a);
    }

    // f * a
    constexpr auto operator*(const function_c auto & f, const real & a)
    {
        return a * f;
    }

    // f / a
    constexpr auto operator/(const function_c auto & f, const real & a)
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
    constexpr auto operator-(const real & a, const function_c auto & f)
    {
        return a + (-f);
    }

    // f - a
    constexpr auto operator-(const function_c auto & f, const real & a)
    {
        return f + (-a);
    }

    // f1 * f2
    constexpr auto operator*(const function_c auto & f1, const function_c auto & f2)
    {
        return Product(f1, f2);
    }

    // a / f
    constexpr auto operator/(const real & a, const function_c auto & f)
    {
        return a * Reciprocal(f);
    }

    // f1 / f2
    constexpr auto operator/(const function_c auto & f1, const function_c auto & f2)
    {
        return f1 * Reciprocal(f2);
    }
}


#endif

// end of file
