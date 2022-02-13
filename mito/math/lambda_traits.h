// code guard
#if !defined(mito_math_lambda_traits_h)
#define mito_math_lambda_traits_h


namespace mito::math {

    template <typename T>
    struct lambda_traits : public lambda_traits<decltype(&T::operator())> {};

    template <typename ClassT, typename ReturnT, typename ArgumentT>
    struct lambda_traits<ReturnT (ClassT::*)(ArgumentT) const> {
        typedef ReturnT result_type;
        typedef ArgumentT argument_type;
    };
}


#endif

// end of file
