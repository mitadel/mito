// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_math_function_library_h)
#define mito_math_function_library_h


// Library of Functions


namespace mito::math {

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

}


#endif

// end of file
