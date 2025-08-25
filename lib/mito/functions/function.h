// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#pragma once


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


    // a function wrapping a functor (i.e. a type that implements the call operator)
    template <functor_c F>
    class FunctionFromFunctor : public function_from_functor<F>::type {
      public:
        // the type of the function (what I derive from)
        using function_type = function_from_functor<F>::type;
        // the input type
        using input_type = function_type::input_type;
        // the output type
        using output_type = function_type::output_type;

      public:
        // constructor
        constexpr FunctionFromFunctor(F && f) : _f(std::forward<F>(f)) {}

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(const input_type & x) const -> output_type { return _f(x); }

      private:
        // the functor
        const F _f;
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
        constexpr auto constant() const -> const output_type & { return _c; }

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(const input_type &) const -> output_type { return _c; }

      private:
        // the constant
        const output_type _c;
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
        constexpr auto operator()(const input_type & x) const -> output_type
        {
            return _f(x) + _g(x);
        }

        // the first in the sum
        constexpr auto f1() const -> const F & { return _f; }

        // the second in the sum
        constexpr auto f2() const -> const G & { return _g; }

      private:
        const F _f;
        const G _g;
    };

    // the linear combination of functions {Funcs} with coefficients of type {T}...
    template <typename T, function_c... Funcs>
    class LinearCombination : public function_linear_combination<T, Funcs...>::type {
      public:
        // the type of the function (what I derive from)
        using function_type = function_linear_combination<T, Funcs...>::type;
        // the input type
        using input_type = function_type::input_type;
        // the output type
        using output_type = function_type::output_type;

      private:
        // the number of functions in the linear combination
        static constexpr std::size_t N = sizeof...(Funcs);
        // the type of coefficients
        using coefficients_type = std::array<T, N>;
        // the type of functions
        using functions_type = std::tuple<Funcs...>;

      public:
        // constructor
        constexpr LinearCombination(const coefficients_type & coeffs, Funcs... funcs) :
            _coeffs(coeffs),
            _funcs(funcs...)
        {}

        // call operator
        constexpr auto operator()(const input_type & x) const -> output_type
        {
            // helper function to assemble the linear combination
            auto eval = [&]<std::size_t... Is>(const input_type & x, std::index_sequence<Is...>) {
                return ((_coeffs[Is] * std::get<Is>(_funcs)(x)) + ...);
            };

            // return the result of the linear combination
            return eval(x, std::make_index_sequence<N>{});
        }

      public:
        // accessor for the coefficients
        constexpr auto coefficients() const -> const coefficients_type & { return _coeffs; }

        // accessor for the functions
        constexpr auto functions() const -> const functions_type & { return _funcs; }

      private:
        // the coefficients in the linear combination
        coefficients_type _coeffs;
        // the functions in the linear combination
        functions_type _funcs;
    };

    // the sum of a function with a constant
    template <tensor::tensor_or_scalar_c T, function_c F>
    class FunctionPlusConstant : public function_type<F> {

      public:
        // the type of function (what I derive from)
        using my_function_type = function_type<F>;
        // the input type of the sum
        using input_type = my_function_type::input_type;
        // the output type of the sum
        using output_type = my_function_type::output_type;

      public:
        // constructor
        constexpr FunctionPlusConstant(const F & f, const T & a) : _f(f), _a(a) {}

        // get the constant
        constexpr auto constant() const -> const T & { return _a; }

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(const input_type & x) const -> output_type { return _f(x) + _a; }

        // the base function
        constexpr auto f() const -> const F & { return _f; }

      private:
        // the function
        const F _f;
        // the constant
        const T _a;
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
        constexpr auto operator()(const input_type & x) const -> output_type
        {
            return _f(x) * _g(x);
        }

        // the first in the sum
        constexpr auto f1() const -> const F & { return _f; }

        // the second in the sum
        constexpr auto f2() const -> const G & { return _g; }

      private:
        const F _f;
        const G _g;
    };


    // the product of a function with a constant
    template <tensor::tensor_or_scalar_c T, function_c F>
    class FunctionTimesConstant :
        public function_product<F, Constant<typename F::input_type, T>>::type {

      public:
        // the type of function (what I derive from)
        using my_function_type = function_product<F, Constant<typename F::input_type, T>>::type;
        // the input type of the sum
        using input_type = my_function_type::input_type;
        // the output type of the sum
        using output_type = my_function_type::output_type;

      public:
        // constructor
        constexpr FunctionTimesConstant(const F & f, const T & a) : _f(f), _a(a) {}

        // get the constant
        constexpr auto constant() const -> const T & { return _a; }

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(const input_type & x) const -> output_type { return _f(x) * _a; }

        // the base function
        constexpr auto f() const -> const F & { return _f; }

      private:
        // the function
        const F _f;
        // the constant
        const T _a;
    };


    // the product of a function with a constant
    template <tensor::tensor_or_scalar_c T, function_c F>
    class ConstantTimesFunction :
        public function_product<Constant<typename F::input_type, T>, F>::type {

      public:
        // the type of function (what I derive from)
        using my_function_type = function_product<Constant<typename F::input_type, T>, F>::type;
        // the input type of the sum
        using input_type = my_function_type::input_type;
        // the output type of the sum
        using output_type = my_function_type::output_type;

      public:
        // constructor
        constexpr ConstantTimesFunction(const T & a, const F & f) : _f(f), _a(a) {}

        // get the constant
        constexpr auto constant() const -> const T & { return _a; }

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(const input_type & x) const -> output_type { return _a * _f(x); }

        // the base function
        constexpr auto f() const -> const F & { return _f; }

      private:
        // the function
        const F _f;
        // the constant
        const T _a;
    };


    // the reciprocal of a scalar function
    template <scalar_function_c F>
    class Reciprocal : public function_type<F> {

      public:
        // the type of function (what I derive from)
        using my_function_type = function_type<F>;
        // the input type of the sum
        using input_type = my_function_type::input_type;
        // the output type of the sum
        using output_type = my_function_type::output_type;

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
        constexpr auto operator()(const input_type & x) const -> output_type { return 1.0 / _f(x); }

        // the base function
        constexpr auto f() const -> const F & { return _f; }

      private:
        const F _f;
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
        constexpr auto operator()(const input_type & x) const -> output_type { return _f(_g(x)); }

        // the outer in the composition
        constexpr auto f1() const -> const F & { return _f; }

        // the inner in the composition
        constexpr auto f2() const -> const G & { return _g; }

      private:
        const F _f;
        const G _g;
    };

    // function transposing a function of a second order tensor
    template <function_c F>
    requires(tensor::matrix_c<typename F::output_type>)
    class Transpose : public function_transpose<F>::type {

      public:
        // the transpose function type
        using transpose_type = function_transpose<F>::type;
        // the input type
        using input_type = transpose_type::input_type;
        // the output type
        using output_type = transpose_type::output_type;

      public:
        // constructor
        constexpr Transpose(const F & f) : _f(f) {}

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(const input_type & x) const -> output_type
        {
            return tensor::transpose(_f(x));
        }

        // the function to transpose
        constexpr auto f() const -> const F & { return _f; }

      private:
        const F _f;
    };

    // function computing the inverse of a second order tensor
    template <function_c F>
    requires(tensor::square_matrix_c<typename F::output_type>)
    class Inverse : public Function<typename F::input_type, typename F::output_type> {

      public:
        // the inverse function type
        using inverse_type = Function<typename F::input_type, typename F::output_type>;
        // the input type
        using input_type = inverse_type::input_type;
        // the output type
        using output_type = inverse_type::output_type;

      public:
        // constructor
        constexpr Inverse(const F & f) : _f(f) {}

        // call operator for function composition
        template <function_c H>
        constexpr auto operator()(const H & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(const input_type & x) const -> output_type
        {
            return tensor::inverse(_f(x));
        }

        // the matrix function to invert
        constexpr auto f() const -> const F & { return _f; }

      private:
        const F _f;
    };
}


// end of file
