// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_api_h)
#define mito_functions_api_h


namespace mito::functions {

    // functions factory
    template <class F>
    constexpr auto function(F && f);

    // // make a vector-valued function from N scalar-valued functions
    // template <class X, int N>
    // constexpr auto function(const function_t<X, scalar_t> (&f_list)[N]);

    // supported math functions
    constexpr auto sin();
    constexpr auto cos();
}


#endif

// end of file
