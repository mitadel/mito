// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_forward_h)
#define mito_functions_forward_h


namespace mito::functions {

    // class {Function}
    template <class X, class Y>
    class Function;

    // a function mapping a {real} to a {real}
    using ScalarFunction = Function<real, real>;

    // a function mapping a type {X} to a {real}
    template <class X>
    using ScalarValuedFunction = Function<X, real>;

    // concept of a function
    template <class F>
    concept function_c = requires(F c) {
        // require that F only binds to {Function<X, Y>} specializations or derived classes
        []<class X, class Y>(Function<X, Y> &) {
        }(c);
    };

    // concept of a scalar-valued function
    template <class F>
    concept scalar_function_c = requires(F c) {
        // require that F only binds to {ScalarValuedFunction} or derived classes
        []<class X>(ScalarValuedFunction<X> &) {
        }(c);
    };

    // function composition
    template <function_c F, function_c G>
    class Composition;

}


#endif

// end of file
