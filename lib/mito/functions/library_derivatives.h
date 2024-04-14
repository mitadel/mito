// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_derivatives_library_h)
#define mito_functions_derivatives_library_h


namespace mito::functions {

    // the {I...}-th first partial derivative of the {Zero} function
    template <int... I, class T>
    constexpr auto derivative(const Zero<T> &)
    {
        return Zero<T>();
    }

    // the {I...}-th first partial derivative of the {One} function
    template <int... I, class T>
    constexpr auto derivative(const One<T> &)
    {
        return Zero<T>();
    }

    // the {I...}-th first partial derivative of the {Power<N>} function
    template <int... I, int N>
    constexpr auto derivative(const Power<N> &)
    {
        return N * pow<N - 1>;
    }

    // the {I...}-th first partial derivative of the {Power<1>} function
    template <int... I>
    constexpr auto derivative(const Power<1> &)
    {
        return one;
    }

    // the {I...}-th first partial derivative of the {Sin} function
    template <int... I>
    constexpr auto derivative(const Sin &)
    {
        return cos;
    }

    // the {I...}-th first partial derivative of the {Cos} function
    template <int... I>
    constexpr auto derivative(const Cos &)
    {
        return -sin;
    }

    // the {I...}-th first partial derivative of the {Component} function
    template <int... I, tensor_c T, int... N>
    constexpr auto derivative(const Component<T, N...> &)
    {
        // the dirac delta
        if constexpr (std::make_tuple(I...) == std::make_tuple(N...))
            return One<T>();
        else
            return Zero<T>();
    }
}


#endif

// end of file
