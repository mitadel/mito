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


    template <class F, class G>
    requires(Function<F> && Function<G>)
    class Sum : public ScalarFunction {
      public:
        // constructor
        constexpr Sum(const F & f, const G & g) : _f(f), _g(g) {}

        // call operator
        constexpr auto operator()(X x) const -> Y { return _f(x) + _g(x); }

        // the first in the sum
        constexpr auto f1() const -> F { return _f; }

        // the second in the sum
        constexpr auto f2() const -> G { return _g; }

      private:
        F _f;
        G _g;
    };


    template <class F>
    requires Function<F>
    class FunctionTimesScalar : public ScalarFunction {
      public:
        // constructor
        constexpr FunctionTimesScalar(const F & f, const double & a) : _f(f), _a(a) {}

        // get the scalar
        constexpr auto scalar() const -> double { return _a; }

        // call operator
        constexpr auto operator()(X x) const -> Y { return _a * _f(x); }

        // the base function
        constexpr auto f() const -> F { return _f; }

      private:
        // the function
        F _f;
        // the scalar
        double _a;
    };
}

#endif    // mito_math_Function_h

// end of file
