// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#if !defined(mito_math_Function_h)
#define mito_math_Function_h


namespace mito::math {


    template <class F>
    concept Function = std::is_base_of_v<ScalarFunction, std::remove_cvref_t<F>>;


    // a function mapping a {real} to a {real}
    class ScalarFunction {
      protected:
        // the input type
        using X = real;
        // the output type
        using Y = real;

      public:
        // default constructor
        constexpr ScalarFunction() = default;
    };


    template <class F>
    requires Function<F>
    class Negative : public ScalarFunction {
      public:
        // constructor
        constexpr Negative(const F & f) : _f(f) {}

        // call operator
        constexpr auto operator()(X x) const -> Y { return -1.0 * _f(x); }

      private:
        F _f;
    };
}

#endif    // mito_math_Function_h

// end of file
