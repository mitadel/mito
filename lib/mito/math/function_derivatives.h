// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_math_function_derivatives_h)
#define mito_math_function_derivatives_h


// Derivatives of Functions


namespace mito::math {

    template <class F>
    constexpr auto _derivative();

    // the first derivative of {f}
    constexpr auto derivative(const Function auto & f)
    {
        return _derivative<std::remove_cvref_t<decltype(f)>>();
    }

    // the first derivative of the negative of a function {f}
    template <class F>
    constexpr auto derivative(const Negative<F> &)
    {
        return -_derivative<F>();
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
}


#endif

// end of file
