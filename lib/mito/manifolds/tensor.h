// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_tensor_h)
#define mito_manifolds_tensor_h


// tensor operations on forms
namespace mito::manifolds {

    // the tensor product of two one-forms
    constexpr auto tensor(const one_form_c auto & a_tilda, const one_form_c auto & b_tilda)
    {
        // return a {form} that, when contracted with {x} and {y}...
        return mito::manifolds::form<2>(
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
        return mito::manifolds::form<3>(
            [a_tilda, b_tilda,
             c_tilda]<vector_or_dummy_c X, vector_or_dummy_c Y, vector_or_dummy_c Z>(
                const X & x, const Y & y, const Z & z) -> auto {
                // ... returns the {scalar} prescribed by the tensor product
                return a_tilda(x) * b_tilda(y) * c_tilda(z);
            });
    }

    // the tensor product of two fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2>
    constexpr auto tensor(const F1 & fA, const F2 & fB)
    requires(compatible_fields_c<F1, F2>)
    {
        using coordinates_type = typename F1::coordinates_type;
        return field(functions::function(
            [fA, fB](const coordinates_type & x) -> auto { return tensor(fA(x), fB(x)); }));
    }

    // the tensor product of three fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2, one_form_field_c F3>
    constexpr auto tensor(const F1 & fA, const F2 & fB, const F3 & fC)
    requires(compatible_fields_c<F1, F2> && compatible_fields_c<F1, F3>)
    {
        using coordinates_type = typename F1::coordinates_type;
        return field(functions::function([fA, fB, fC](const coordinates_type & x) -> auto {
            return tensor(fA(x), fB(x), fC(x));
        }));
    }
}


#endif

// end of file
