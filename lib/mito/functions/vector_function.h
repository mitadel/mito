// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//


// code guard
#if !defined(mito_functions_vector_function_h)
#define mito_functions_vector_function_h


namespace mito::functions {

    // a function mapping a {vector_t<D, real>} to a {real}
    template <int D>
    requires(D > 1)
    class VectorFunction {

      protected:
        // the input type
        using X = vector_t<D>;

        // the output type
        using Y = real;
    };

}

#endif    // mito_functions_Function_h

// end of file
