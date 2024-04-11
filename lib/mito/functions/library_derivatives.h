// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_derivatives_library_h)
#define mito_functions_derivatives_library_h


namespace mito::functions {

    template <>
    constexpr auto _derivative<Zero>()
    {
        return zero;
    }

    template <>
    constexpr auto _derivative<One>()
    {
        return zero;
    }

    template <int N>
    constexpr auto derivative(const Power<N> &)
    {
        return N * pow<N - 1>;
    }

    template <>
    constexpr auto derivative(const Power<1> &)
    {
        return one;
    }

    template <>
    constexpr auto _derivative<Sin>()
    {
        return cos;
    }

    template <>
    constexpr auto _derivative<Cos>()
    {
        return -sin;
    }
}


#endif

// end of file
