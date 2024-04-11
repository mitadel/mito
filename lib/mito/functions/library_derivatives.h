// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_derivatives_library_h)
#define mito_functions_derivatives_library_h


namespace mito::functions {

    constexpr auto derivative(const Zero &)
    {
        return zero;
    }

    constexpr auto derivative(const One &)
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

    template <int I, int N, int D>
    requires(I <= D)
    constexpr auto derivative(const Component<N, D> &)
    {
        // the dirac delta
        if constexpr (I == N)
            return 1.0;
        else
            return 0.0;
    }
}


#endif

// end of file
