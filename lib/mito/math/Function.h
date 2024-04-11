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
    class FunctionPlusConstant : public ScalarFunction {
      public:
        // constructor
        constexpr FunctionPlusConstant(const F & f, const double & a) : _f(f), _a(a) {}

        // get the scalar
        constexpr auto scalar() const -> double { return _a; }

        // call operator
        constexpr auto operator()(X x) const -> Y { return _f(x) + _a; }

        // the base function
        constexpr auto f() const -> F { return _f; }

      private:
        // the function
        F _f;
        // the scalar
        double _a;
    };


    template <class F>
    requires Function<F>
    class FunctionTimesConstant : public ScalarFunction {
      public:
        // constructor
        constexpr FunctionTimesConstant(const F & f, const double & a) : _f(f), _a(a) {}

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


    template <class F, class G>
    requires(Function<F> && Function<G>)
    class Product : public ScalarFunction {
      public:
        // constructor
        constexpr Product(const F & f, const G & g) : _f(f), _g(g) {}

        // call operator
        constexpr auto operator()(X x) const -> Y { return _f(x) * _g(x); }

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
    class Reciprocal : public ScalarFunction {
      public:
        // constructor
        constexpr Reciprocal(const F & f) : _f(f) {}

        // call operator
        constexpr auto operator()(X x) const -> Y { return 1.0 / _f(x); }

        // the base function
        constexpr auto f() const -> F { return _f; }

      private:
        F _f;
    };
}

#endif    // mito_math_Function_h

// end of file
