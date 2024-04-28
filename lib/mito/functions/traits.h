// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_traits_h)
#define mito_functions_traits_h


namespace mito::functions {

    // the type of the sum of two functions...
    template <function_c F, function_c G>
    // ... which take the same input type {input_type}...
    requires(std::is_same_v<typename F::input_type, typename G::input_type>)
    struct function_sum {
        // ... is the function that takes {input_type} in input and returns the type of the sum of
        // the output types
        using type = Function<
            typename F::input_type, typename std::result_of<std::plus<>(
                                        typename F::output_type, typename G::output_type)>::type>;
    };

    // the type of the product of two functions...
    template <function_c F, function_c G>
    // ... which take the same input type {input_type}...
    requires(std::is_same_v<typename F::input_type, typename G::input_type>)
    struct function_product {
        // ... is the function that takes {input_type} in input and returns the type of the product
        // of the output types
        using type = Function<
            typename F::input_type, typename std::result_of<std::multiplies<>(
                                        typename F::output_type, typename G::output_type)>::type>;
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
            typename functor_traits<F>::input_type, typename functor_traits<F>::output_type>;
    };
}


#endif

// end of file
