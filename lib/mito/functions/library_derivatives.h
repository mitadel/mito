// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_derivatives_library_h)
#define mito_functions_derivatives_library_h


namespace mito::functions {

    // the {I...}-th first partial derivative of the {Constant} function
    template <int... I, class X, class Y>
    constexpr auto derivative(const Constant<X, Y> &)
    {
        // the zero constant
        return Constant<X, Y>({});
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
        return one<scalar_t>;
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

    // the {I...}-th first partial derivative of the {Tan} function
    template <int... I>
    constexpr auto derivative(const Tan &)
    {
        return 1.0 / pow<2>(cos);
    }

    // the {I...}-th first partial derivative of the {ArcCos} function
    template <int... I>
    constexpr auto derivative(const ArcCos &)
    {
        return -1.0 / sqrt(1.0 - pow<2>);
    }

    // the {I...}-th first partial derivative of the {ArcSin} function
    template <int... I>
    constexpr auto derivative(const ArcSin &)
    {
        return 1.0 / sqrt(1.0 - pow<2>);
    }

    // the {I...}-th first partial derivative of the {ArcTan} function
    template <int... I>
    constexpr auto derivative(const ArcTan &)
    {
        return 1.0 / (1.0 + pow<2>);
    }

    // the {I...}-th first partial derivative of the {Exp} function
    template <int... I>
    constexpr auto derivative(const Exp &)
    {
        return exp;
    }

    // the {I...}-th first partial derivative of the {Log} function
    template <int... I>
    constexpr auto derivative(const Log &)
    {
        return 1.0 / pow<1>;
    }

    // the {I...}-th first partial derivative of the {Sqrt} function
    template <int... I>
    constexpr auto derivative(const Sqrt &)
    {
        return 0.5 / sqrt;
    }

    // the {I...}-th first partial derivative of the {Cbrt} function
    template <int... I>
    constexpr auto derivative(const Cbrt &)
    {
        return (1.0 / 3.0) / pow<2>(cbrt);
    }

    // the {I...}-th first partial derivative of the {Component} function
    template <int... I, class T, int... N>
    constexpr auto derivative(const Component<T, N...> &)
    {
        // the dirac delta
        if constexpr (std::make_tuple(I...) == std::make_tuple(N...))
            return one<T>;
        else
            return zero<T>;
    }

}


#endif

// end of file
