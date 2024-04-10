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
    constexpr auto _derivative<Negative<Sin>>()
    {
        return -_derivative<Sin>();
    }

    template <>
    constexpr auto _derivative<Negative<Cos>>()
    {
        return -_derivative<Cos>();
    }

    constexpr auto derivative(const Function auto & f)
    {
        return _derivative<std::remove_cvref_t<decltype(f)>>();
    }
}


#endif

// end of file
