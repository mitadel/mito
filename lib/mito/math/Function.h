
// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h


namespace mito::math {

    // This class provides a single interface to lambda functions, {std::function}, and pointers to
    // functions. The class template parameter is the type of the function wrapped.

    template <class F>
    class Function {
      private:
        // type traits on function type
        using traits = std::conditional<
            // if {F} is a pointer to function
            std::is_pointer<F>::value,
            // use the traits for pointer to functions
            pointer_function_traits<F>,
            // otherwise use the traits for a lambda (these traits also work on {std::function})
            lambda_traits<F>>::type;

      public:
        // the input type
        using X = typename traits::argument_type;
        // the output type
        using Y = typename traits::result_type;

      public:
        // constructor
        constexpr Function(F f) : _f { f } {}

        // call operator
        constexpr auto operator()(X x) const -> Y { return _f(x); }

      private:
        // the function wrapped
        F _f;
    };
}

#endif    // mito_math_Function_h

// end of file
