// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_forward_h)
#define mito_functions_forward_h


namespace mito::functions {

    // the functor concept
    template <class F>
    concept functor_c = requires { &F::operator(); };

    // class {Function}
    template <class X, class Y>
    class Function;

    // a function mapping a type {X} to a {scalar_t}
    template <tensor_or_scalar_c X>
    using ScalarValuedFunction = Function<X, scalar_t>;

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
