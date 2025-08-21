// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::functions {

    // the type of the product of {C} and {X}
    template <typename C, typename X>
    using product_result_t = std::invoke_result_t<std::multiplies<>, C, X>;

    // helper: fold "plus" over a pack of types
    template <typename... Ts>
    struct sum_result;

    // helper: base case
    template <typename T>
    struct sum_result<T> {
        using type = T;
    };

    // helper: recursion
    template <typename T1, typename T2, typename... Rest>
    struct sum_result<T1, T2, Rest...> {
        using type = std::invoke_result_t<std::plus<>, T1, typename sum_result<T2, Rest...>::type>;
    };

    // the type of the sum of {Ts...}
    template <typename... Ts>
    using sum_result_t = typename sum_result<Ts...>::type;

    // the type of the sum of two functions...
    template <function_c F, function_c G>
    // ... which take the same input type {input_type}...
    requires(std::is_same_v<typename F::input_type, typename G::input_type>)
    struct function_sum {
        // ... is the function that takes {input_type} in input and returns the type of the sum of
        // the output types
        using type = Function<
            typename F::input_type, sum_result_t<typename F::output_type, typename G::output_type>>;
    };

    // the type of the product of two functions...
    template <function_c F, function_c G>
    // ... which take the same input type {input_type}...
    requires(std::is_same_v<typename F::input_type, typename G::input_type>)
    struct function_product {
        // ... is the function that takes {input_type} in input and returns the type of the product
        // of the output types
        using type = Function<
            typename F::input_type,
            product_result_t<typename F::output_type, typename G::output_type>>;
    };

    // the type of the composition of two functions (F external, G internal)...
    template <function_c F, function_c G>
    // ... which take compatible input types (i.e. output of G is convertible to input of F)...
    requires(std::convertible_to<typename G::output_type, typename F::input_type>)
    struct function_composition {
        // ... is the function that takes the {input_type} of G in input and returns the output type
        // of F
        using type = Function<typename G::input_type, typename F::output_type>;
    };

    // the type of the transpose of a function...
    template <function_c F>
    // with matrix values...
    requires(tensor::matrix_c<typename F::output_type>)
    struct function_transpose {
        // ... is the function that takes {input_type} in input and returns the type of the
        // transposed of the output matrix type
        using type = Function<
            typename F::input_type,
            typename tensor::traits::transpose<typename F::output_type>::type>;
    };

    // the type of function for a function of class {F}
    template <function_c F>
    using function_type = Function<typename F::input_type, typename F::output_type>;

    // traits for functors
    template <typename T>
    struct functor_traits : public functor_traits<decltype(&T::operator())> {};

    template <typename ClassT, typename ReturnT, typename ArgumentT>
    struct functor_traits<ReturnT (ClassT::*)(ArgumentT) const> {
        using output_type = ReturnT;
        using input_type = ArgumentT;
    };

    // the function type of a functor
    template <functor_c F>
    struct function_from_functor {
        using type = Function<
            std::remove_cvref_t<typename functor_traits<F>::input_type>,
            typename functor_traits<F>::output_type>;
    };
}


// end of file
