// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


// Algebraic operations on Fields
namespace mito::fields {

    // det(f)
    template <tensor_field_c F>
    constexpr auto determinant(const F & f)
    {
        using coordinates_type = typename F::input_type;
        return functions::function([f](const coordinates_type & x) -> mito::tensor::scalar_t {
            return determinant(f(x));
        });
    }

    // the tensor product of two fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2>
    constexpr auto tensor(const F1 & fA, const F2 & fB)
    requires(compatible_fields_c<F1, F2>)
    {
        using coordinates_type = typename F1::input_type;
        return functions::function(
            [fA, fB](const coordinates_type & x) -> auto { return tensor::tensor(fA(x), fB(x)); });
    }

    // the tensor product of three fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2, one_form_field_c F3>
    constexpr auto tensor(const F1 & fA, const F2 & fB, const F3 & fC)
    requires(compatible_fields_c<F1, F2> && compatible_fields_c<F1, F3>)
    {
        using coordinates_type = typename F1::input_type;
        return field(functions::function([fA, fB, fC](const coordinates_type & x) -> auto {
            return tensor::tensor(fA(x), fB(x), fC(x));
        }));
    }

    // the wedge product of one field of one-forms (trivial case)
    constexpr auto wedge(const one_form_field_c auto & fA)
    {
        // return a {fA}
        return fA;
    }

    // the wedge product of two fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2>
    constexpr auto wedge(const F1 & fA, const F2 & fB)
    requires(compatible_fields_c<F1, F2>)
    {
        using coordinates_type = typename F1::input_type;
        return functions::function(
            [fA, fB](const coordinates_type & x) { return tensor::wedge(fA(x), fB(x)); });
    }

    // the wedge product of three fields of one-forms
    template <one_form_field_c F1, one_form_field_c F2, one_form_field_c F3>
    constexpr auto wedge(const F1 & fA, const F2 & fB, const F3 & fC)
    requires(compatible_fields_c<F1, F2> && compatible_fields_c<F1, F3>)
    {
        using coordinates_type = typename F1::input_type;
        return functions::function([fA, fB, fC](const coordinates_type & x) {
            return tensor::wedge(fA(x), fB(x), fC(x));
        });
    }
}


// end of file
