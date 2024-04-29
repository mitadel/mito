// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_fields_differential_h)
#define mito_fields_differential_h


// Differential operators on Fields
namespace mito::fields {

    // the {I...}-th first partial derivative of a field
    template <int... I>
    constexpr auto derivative(const field_c auto & f)
    {
        // the {I...}-th first partial derivative
        return field(mito::functions::derivative<I...>(f.function()));
    }
}


#endif

// end of file
