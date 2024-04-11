// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_forward_h)
#define mito_functions_forward_h


namespace mito::functions {

    // class for a scalar function
    class ScalarFunction;

    // zero function
    class Zero;
    // one function
    class One;
    // sine function
    class Sin;
    // cosine function
    class Cos;

    // the derivative of a function of class {F}
    template <class F>
    constexpr auto _derivative();
}


#endif

// end of file
