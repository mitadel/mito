// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_functions_functions_library_h)
#define mito_functions_functions_library_h


// Library of Functions


namespace mito::functions {

    class Zero : public ScalarFunction {
      public:
        // constructor
        constexpr Zero() = default;

        // call operator
        constexpr auto operator()(X) const -> Y { return 0.0; }
    };

    class One : public ScalarFunction {
      public:
        // constructor
        constexpr One() = default;

        // call operator
        constexpr auto operator()(X) const -> Y { return 1.0; }
    };

    class Sin : public ScalarFunction {
      public:
        // constructor
        constexpr Sin() = default;

        // call operator
        constexpr auto operator()(X x) const -> Y { return std::sin(x); }
    };

    class Cos : public ScalarFunction {
      public:
        // constructor
        constexpr Cos() = default;

        // call operator
        constexpr auto operator()(X x) const -> Y { return std::cos(x); }
    };

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
        // constructor
        constexpr Power() = default;

        // call operator
        constexpr auto operator()(X x) const -> Y { return _power<N>(x); }
    };
}


#endif

// end of file
