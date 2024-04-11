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

    // concept of a scalar function
    template <class F>
    concept function_c = std::is_base_of_v<ScalarFunction, std::remove_cvref_t<F>>;

    // function composition
    template <function_c F, function_c G>
    class Composition;

}


#endif

// end of file
