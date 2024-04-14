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
}


#endif

// end of file
