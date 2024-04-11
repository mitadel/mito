// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_functions_library_h)
#define mito_functions_functions_library_h


// Library of Functions


namespace mito::functions {

    // the zero function
    class Zero : public ScalarFunction {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(X) const -> Y { return 0.0; }
    };


    // the identity function
    class One : public ScalarFunction {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(X) const -> Y { return 1.0; }
    };


    // the sine function
    class Sin : public ScalarFunction {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(X x) const -> Y { return std::sin(x); }
    };


    // the cosine function
    class Cos : public ScalarFunction {

      public:
        // call operator for function composition
        template <function_c F>
        constexpr auto operator()(const F & f) const
        {
            return Composition(*this, f);
        }

        // call operator
        constexpr auto operator()(X x) const -> Y { return std::cos(x); }
    };


    // the power to integer functions
    template <int N>
    requires(N >= 1)
    class Power : public ScalarFunction {

      private:
        template <int I>
        constexpr auto _power(X x) const
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
        constexpr auto operator()(X x) const -> Y { return _power<N>(x); }
    };
}


#endif

// end of file
