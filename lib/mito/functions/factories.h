// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_factories_h)
#define mito_functions_factories_h


namespace mito::functions {

    // returns the function mapping an input of type {X} to the constant {c}
    template <class X, class Y>
    constexpr auto constant(const Y & c)
    {
        return Constant<X, Y>(c);
    }

    // returns the function wrapping the functor {f}
    template <functor_c F>
    constexpr auto function(F && f)
    {
        return FunctionFromFunctor<F>(std::move(f));
    }
}


#endif

// end of file
