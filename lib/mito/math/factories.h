// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_math_factories_h)
#define mito_math_factories_h


namespace mito::math {

    // factory returning a constant function
    template <class X, real c>
    constexpr auto constant()
    {
        return function([](const X &) { return c; });
    }

    // factory returning the function {f(x) = 1.0}
    template <class X>
    constexpr auto one()
    {
        return constant<X, 1.0>();
    }

    // functions factory
    template <class F>
    constexpr auto function(F && f)
    {
        return function_t<F>(std::forward<F>(f));
    }

    // TOFIX: do we need this?
    // // make a vector-valued function from N scalar-valued functions
    // template <class X, int N>
    // constexpr auto function(const function_t<X, scalar_t> (&f_list)[N])
    // {
    //     return function_t<X, vector_t<N>>(f_list);
    // }

    constexpr auto sin()
    {
        return Sin();
    }

    constexpr auto cos()
    {
        return Cos();
    }
}


#endif

// end of file
