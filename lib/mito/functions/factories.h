// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_factories_h)
#define mito_functions_factories_h


namespace mito::functions {

    // returns the function mapping an input of type {X} to the constant {c}
    template <tensor_or_scalar_c X, tensor_or_scalar_c Y>
    constexpr auto constant(const Y & c)
    {
        return Constant<X, Y>(c);
    }
}


#endif

// end of file
