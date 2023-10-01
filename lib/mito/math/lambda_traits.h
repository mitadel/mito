// code guard
#if !defined(mito_math_lambda_traits_h)
#define mito_math_lambda_traits_h


namespace mito::math {

    // traits for lambda function
    template <typename T>
    struct lambda_traits : public lambda_traits<decltype(&T::operator())> {};

    template <typename ClassT, typename ReturnT, typename ArgumentT>
    struct lambda_traits<ReturnT (ClassT::*)(ArgumentT) const> {
        typedef ReturnT result_type;
        typedef ArgumentT argument_type;
    };

    // helper typedef to remove the reference to F&&
    template <class F>
    using remove_reference_lambda = typename std::remove_reference_t<F>;

    // traits for pointers to function
    template <typename T>
    struct pointer_function_traits : public pointer_function_traits<decltype(&T::operator())> {};

    template <typename ReturnT, typename ArgumentT>
    struct pointer_function_traits<ReturnT (*)(ArgumentT)> {
        typedef ReturnT result_type;
        typedef ArgumentT argument_type;
    };
}


#endif

// end of file
