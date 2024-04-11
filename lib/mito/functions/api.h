// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_api_h)
#define mito_functions_api_h


namespace mito::functions {

    // supported math functions
    constexpr auto zero = Zero();
    constexpr auto one = One();
    constexpr auto sin = Sin();
    constexpr auto cos = Cos();
    constexpr auto tan = sin / cos;
    constexpr auto sec = 1.0 / cos;
    constexpr auto csec = 1.0 / sin;
}


#endif

// end of file
