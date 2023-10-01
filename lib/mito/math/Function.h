
// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h


namespace mito::math {

    template <class F>
    class Function {

      private:
        using traits = std::conditional<
            std::is_pointer<F>::value, pointer_function_traits<F>, lambda_traits<F>>::type;

      public:
        using X = typename traits::argument_type;
        using Y = typename traits::result_type;

      public:
        constexpr Function(F f) : _f { f } {}
        constexpr auto operator()(X x) const -> Y { return _f(x); }

      private:
        F _f;
    };
}

#endif    // mito_math_Function_h

// end of file
