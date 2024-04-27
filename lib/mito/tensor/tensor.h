// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_tensor_tensor_h)
#define mito_tensor_tensor_h


// tensor operations on forms
namespace mito::tensor {

    // the tensor product of two one-forms
    constexpr auto tensor(const one_form_c auto & a_tilda, const one_form_c auto & b_tilda)
    {
        // return a {form} that, when contracted with {x} and {y}...
        return form<2>(
            [a_tilda, b_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y>(
                const X & x, const Y & y) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y);
            });
    }

    // the tensor product of three one-forms
    constexpr auto tensor(
        const one_form_c auto & a_tilda, const one_form_c auto & b_tilda,
        const one_form_c auto & c_tilda)
    {
        // return a {form} that, when contracted with {x}, {y} and {z}...
        return form<3>(
            [a_tilda, b_tilda,
             c_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y, vector_or_dummy_c Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y) * c_tilda(z);
            });
    }
}


#endif

// end of file
