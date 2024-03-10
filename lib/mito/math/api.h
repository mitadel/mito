// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_math_api_h)
#define mito_math_api_h


namespace mito::math {

    // functions factory
    template <class F>
    constexpr auto function(F && f);

    // // make a vector-valued function from N scalar-valued functions
    // template <class X, int N>
    // constexpr auto function(const function_t<X, scalar_t> (&f_list)[N]);
}


#endif

// end of file
