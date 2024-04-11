// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_factories_h)
#define mito_functions_factories_h


namespace mito::functions {

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

    constexpr auto zero()
    {
        return Zero();
    }

    constexpr auto one()
    {
        return One();
    }

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
