
// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h


namespace mito::math {

    template <class F>
    class Function {

      private:
        // TOFIX: remove remove_reference if useless
        using traits = lambda_traits<remove_reference_lambda<F>>;

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
