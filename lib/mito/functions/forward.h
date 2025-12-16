// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::functions {

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

    // the functor concept
    template <class F>
    concept functor_c = requires { &F::operator(); } and not function_c<F>;

    // concept of a function defined on scalars
    template <class F>
    concept scalar_domain_function_c = function_c<F> and tensor::scalar_c<typename F::input_type>;

    // concept of a scalar-valued function
    template <class F>
    concept scalar_valued_function_c = function_c<F> and tensor::scalar_c<typename F::output_type>;

    // concept of a scalar-valued function of scalars
    template <class F>
    concept scalar_function_c = scalar_domain_function_c<F> and scalar_valued_function_c<F>;

    // concept of a function defined on vectors
    template <class F>
    concept vector_domain_function_c = function_c<F> and tensor::vector_c<typename F::input_type>;

    // concept of a vector-valued function
    template <class F>
    concept vector_valued_function_c = function_c<F> and tensor::vector_c<typename F::output_type>;

    // concept of a vector-valued function of vectors
    template <class F>
    concept vector_function_c = vector_domain_function_c<F> and vector_valued_function_c<F>;

    // concept of a function defined on tensors or coordinates
    template <class F>
    concept tensor_domain_function_c = function_c<F>
                                   and (tensor::tensor_c<typename F::input_type>
                                        or geometry::coordinates_c<typename F::input_type>);

    // concept of a tensor-valued function
    template <class F>
    concept tensor_valued_function_c = function_c<F> and tensor::tensor_c<typename F::output_type>;

    // concept of a tensor-valued function of tensors
    template <class F>
    concept tensor_function_c = tensor_domain_function_c<F> and tensor_valued_function_c<F>;

    // concept of a subscriptable type
    template <class F>
    concept subscriptable_c = requires(F f, int i) {
        { f[i] };
    };

    // concept of a subscriptable function
    template <class F>
    concept subscriptable_function_c = function_c<F> and subscriptable_c<typename F::output_type>;

    // concept of functions taking the same input type
    template <typename... Funcs>
    concept same_input_c = (sizeof...(Funcs) <= 1) ||    // trivially true for 0 or 1
                           (std::same_as<
                                typename std::tuple_element_t<0, std::tuple<Funcs...>>::input_type,
                                typename Funcs::input_type>
                            && ...);

    // function composition
    template <function_c F, function_c G>
    class Composition;

    // function subscript
    template <subscriptable_function_c F, typename indexT>
    class Subscript;
}


// end of file
