// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::functions {

    // the functor concept
    template <class F>
    concept functor_c = requires { &F::operator(); };

    // class {Function}
    template <class X, class Y>
    class Function;

    // concept of a function
    template <class F>
    concept function_c = requires(F c) {
        // require that F only binds to {Function<X, Y>} specializations or derived classes
        []<class X, class Y>(const Function<X, Y> &) {
        }(c);
    };

    // concept of a scalar-valued function
    template <class F>
    concept scalar_function_c = function_c<F> and scalar_c<typename F::output_type>;

    // function composition
    template <function_c F, function_c G>
    class Composition;

}


// end of file
