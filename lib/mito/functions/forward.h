// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_forward_h)
#define mito_functions_forward_h


namespace mito::functions {

    // concept for a tensor (vector, matrix, higher-order, ...) or a scalar
    template <class F>
    concept tensor_or_scalar_c = tensor_c<F> or std::convertible_to<F, scalar_t>;

    // class {Function}
    template <tensor_or_scalar_c X, tensor_or_scalar_c Y>
    class Function;

    // a function mapping a {scalar_t} to a {scalar_t}
    using ScalarFunction = Function<scalar_t, scalar_t>;

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
