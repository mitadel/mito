// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_tensor_wedge_h)
#define mito_tensor_wedge_h


// wedge operator on forms
namespace mito::tensor {

    // the wedge product of one one-form (trivial case)
    constexpr auto wedge(const one_form_c auto & a_tilda)
    {
        // return a {a_tilda}
        return a_tilda;
    }

    // the wedge product of two one-forms
    constexpr auto wedge(const one_form_c auto & a_tilda, const one_form_c auto & b_tilda)
    {
        // return a {form} that, when contracted with {x} and {y}...
        return tensor(a_tilda, b_tilda) - tensor(b_tilda, a_tilda);
    }

    // the wedge product of three one-forms
    constexpr auto wedge(
        const one_form_c auto & a_tilda, const one_form_c auto & b_tilda,
        const one_form_c auto & c_tilda)
    {
        // return a {form} that, when contracted with {x}, {y} and {z}...
        return form<3>(
            [a_tilda, b_tilda,
             c_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y, vector_or_dummy_c Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the wedge product
                return a_tilda(x) * wedge(b_tilda, c_tilda)(y, z)
                     + b_tilda(x) * wedge(c_tilda, a_tilda)(y, z)
                     + c_tilda(x) * wedge(a_tilda, b_tilda)(y, z);
            });
    }
}


#endif

// end of file
