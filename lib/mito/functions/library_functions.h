// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_functions_library_h)
#define mito_functions_functions_library_h


// Library of Functions


namespace mito::functions {

    // function extracting the {I...} component of a tensor
    template <tensor_c T, int... I>
    class Component : public Function<T, typename T::type> {

      public:
        // the input type
        using input_type = Function<T, typename T::type>::input_type;
        // the output type
        using output_type = Function<T, typename T::type>::output_type;

        // the tensor of type {T} with a one in the entry whose indices are specified in {J...}
        static constexpr auto e = pyre::tensor::unit<T, I...>;

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return x * e; }
    };


    // the sine function
    class Sin : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::sin(x); }
    };


    // the cosine function
    class Cos : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::cos(x); }
    };


    // the tangent function
    class Tan : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::tan(x); }
    };


    // the arc cosine function
    class ArcCos : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::acos(x); }
    };


    // the arc sine function
    class ArcSin : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::asin(x); }
    };


    // the arc tangent function
    class ArcTan : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::atan(x); }
    };


    // the exponential function
    class Exp : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::exp(x); }
    };


    // the natural logarithm function
    class Log : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::log(x); }
    };


    // the square root function
    class Sqrt : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::sqrt(x); }
    };


    // the cubic root function
    class Cbrt : public Function<scalar_t, scalar_t> {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return std::cbrt(x); }
    };


    // the power to integer functions
    template <int N>
    requires(N >= 1)
    class Power : public Function<scalar_t, scalar_t> {

      private:
        template <int I>
        constexpr auto _power(input_type x) const
        {
            if constexpr (I == 0)
                return 1.0;
            else
                return _power<I - 1>(x) * x;
        }

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(input_type x) const -> output_type { return _power<N>(x); }
    };
}


#endif

// end of file
