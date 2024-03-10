// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_math_lambda_traits_h)
#define mito_math_lambda_traits_h


namespace mito::math {

    // traits for lambda function
    template <typename T>
    struct lambda_traits : public lambda_traits<decltype(&T::operator())> {};

    template <typename ClassT, typename ReturnT, typename ArgumentT>
    struct lambda_traits<ReturnT (ClassT::*)(ArgumentT) const> {
        using result_type = ReturnT;
        using argument_type = ArgumentT;
    };

    // traits for pointers to function
    template <typename T>
    struct pointer_function_traits : public pointer_function_traits<decltype(&T::operator())> {};

    template <typename ReturnT, typename ArgumentT>
    struct pointer_function_traits<ReturnT (*)(ArgumentT)> {
        using result_type = ReturnT;
        using argument_type = ArgumentT;
    };
}


#endif

// end of file
