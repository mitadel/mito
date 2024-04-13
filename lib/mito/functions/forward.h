// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_forward_h)
#define mito_functions_forward_h


namespace mito::functions {

    // class for a scalar function
    template <class X, class Y>
    class Function;

    // a function mapping a {real} to a {real}
    using ScalarFunction = Function<real, real>;

    // a function mapping a {vector_t<D>} to a {real}
    template <int D>
    requires(D > 1)
    using VectorFunction = Function<vector_t<D>, real>;

    // concept of a function
    template <class F>
    concept function_c = requires(F c) {
        // require that F only binds to Function<X, Y> specializations or derived classes
        []<class X, class Y>(Function<X, Y> &) {
        }(c);
    };

    // concept of a scalar function
    template <class F>
    concept scalar_function_c = requires(F c) {
        // require that F only binds to ScalarFunction or derived classes
        [](ScalarFunction &) {
        }(c);
    };

    // function composition
    template <function_c F, function_c G>
    class Composition;

}


#endif

// end of file
