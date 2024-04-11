// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_math_function_derivatives_h)
#define mito_math_function_derivatives_h


// Derivatives of Functions


namespace mito::math {

    // the derivative of a function of class {F}
    template <class F>
    constexpr auto _derivative();

    // the first derivative of {f}
    constexpr auto derivative(const Function auto & f)
    {
        return _derivative<std::remove_cvref_t<decltype(f)>>();
    }

    // the first derivative of a function sum {f}
    template <class F1, class F2>
    constexpr auto derivative(const Sum<F1, F2> & f)
    {
        return derivative(f.f1()) + derivative(f.f2());
    }

    // the first derivative of a scalar times a function {f}
    template <class F>
    constexpr auto derivative(const FunctionTimesScalar<F> & f)
    {
        return f.scalar() * derivative(f.f());
    }

    // the first derivative of a product {f}
    template <class F1, class F2>
    constexpr auto derivative(const Product<F1, F2> & f)
    {
        return f.f1() * derivative(f.f2()) + derivative(f.f1()) * f.f2();
    }

    template <>
    constexpr auto _derivative<Sin>()
    {
        return Cos();
    }

    template <>
    constexpr auto _derivative<Cos>()
    {
        return -Sin();
    }

    template <>
    constexpr auto _derivative<Zero>()
    {
        return Zero();
    }

    template <>
    constexpr auto _derivative<One>()
    {
        return Zero();
    }
}


#endif

// end of file
