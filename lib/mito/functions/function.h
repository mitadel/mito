// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#if !defined(mito_functions_function_h)
#define mito_functions_function_h


namespace mito::functions {

    // a function mapping a {X} instance to a {Y} instance
    template <class X, class Y>
    class Function {
      public:
        // the input type
        using input_type = X;
        // the output type
        using output_type = Y;
    };


    // the constant function
    template <class X, class Y>
    class Constant : public Function<X, Y> {
      public:
        // the input type
        using input_type = Function<X, Y>::input_type;
        // the output type
        using output_type = Function<X, Y>::output_type;

      public:
        // constructor
        constexpr Constant(const output_type & c) : _c(c) {}

        // get the constant
        constexpr auto constant() const -> output_type { return _c; }

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type) const -> output_type { return _c; }

      private:
        // the constant
        output_type _c;
    };


    // the sum of two functions
    template <function_c F, function_c G>
    class Sum : public function_sum<F, G>::type {
      public:
        // the type of sum function (what I derive from)
        using sum_type = function_sum<F, G>::type;
        // the input type of the sum
        using input_type = sum_type::input_type;
        // the output type of the sum
        using output_type = sum_type::output_type;

      public:
        // constructor
        constexpr Sum(const F & f, const G & g) : _f(f), _g(g) {}

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return _f(x) + _g(x); }

        // the first in the sum
        constexpr auto f1() const -> F { return _f; }

        // the second in the sum
        constexpr auto f2() const -> G { return _g; }

      private:
        F _f;
        G _g;
    };


    // the sum of a scalar function with a constant
    template <function_c F>
    class FunctionPlusConstant : public ScalarFunction {
      public:
        // constructor
        constexpr FunctionPlusConstant(const F & f, const double & a) : _f(f), _a(a) {}

        // get the constant
        constexpr auto constant() const -> double { return _a; }

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return _f(x) + _a; }

        // the base function
        constexpr auto f() const -> F { return _f; }

      private:
        // the function
        F _f;
        // the constant
        double _a;
    };


    // the product of two functions
    template <function_c F, function_c G>
    class Product : public function_product<F, G>::type {
      public:
        // the type of product function (what I derive from)
        using product_type = function_product<F, G>::type;
        // the input type of the sum
        using input_type = product_type::input_type;
        // the output type of the sum
        using output_type = product_type::output_type;

      public:
        // constructor
        constexpr Product(const F & f, const G & g) : _f(f), _g(g) {}

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return _f(x) * _g(x); }

        // the first in the sum
        constexpr auto f1() const -> F { return _f; }

        // the second in the sum
        constexpr auto f2() const -> G { return _g; }

      private:
        F _f;
        G _g;
    };


    // the product of a scalar function with a constant
    template <function_c F>
    class FunctionTimesConstant : public ScalarFunction {
      public:
        // constructor
        constexpr FunctionTimesConstant(const F & f, const double & a) : _f(f), _a(a) {}

        // get the constant
        constexpr auto constant() const -> double { return _a; }

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return _a * _f(x); }

        // the base function
        constexpr auto f() const -> F { return _f; }

      private:
        // the function
        F _f;
        // the constant
        double _a;
    };


    // the reciprocal of a scalar function
    template <function_c F>
    class Reciprocal : public ScalarFunction {
      public:
        // constructor
        constexpr Reciprocal(const F & f) : _f(f) {}

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return 1.0 / _f(x); }

        // the base function
        constexpr auto f() const -> F { return _f; }

      private:
        F _f;
    };


    // the composition of two functions
    template <function_c F, function_c G>
    class Composition : public function_composition<F, G>::type {

      public:
        // the type of composition function (what I derive from)
        using composition_type = function_composition<F, G>::type;
        // the input type of the composition
        using input_type = composition_type::input_type;
        // the output type of the composition
        using output_type = composition_type::output_type;

      public:
        // constructor
        constexpr Composition(const F & f, const G & g) : _f(f), _g(g) {}

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return _f(_g(x)); }

        // the outer in the composition
        constexpr auto f1() const -> F { return _f; }

        // the inner in the composition
        constexpr auto f2() const -> G { return _g; }

      private:
        F _f;
        G _g;
    };
}

#endif    // mito_functions_Function_h

// end of file
