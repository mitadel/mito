// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_derivatives_library_h)
#define mito_functions_derivatives_library_h


namespace mito::functions {

    template <class T>
    constexpr auto derivative(const Zero<T> &)
    {
        return zero;
    }

    template <class T>
    constexpr auto derivative(const One<T> &)
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

    constexpr auto derivative(const Sin &)
    {
        return cos;
    }

    constexpr auto derivative(const Cos &)
    {
        return -sin;
    }

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
