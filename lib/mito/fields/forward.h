// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::fields {

    // class field
    template <functions::function_c F>
    requires(geometry::coordinates_c<typename F::input_type>)
    class Field;

    // concept of a field
    template <class FIELD>
    concept field_c = requires(FIELD c) {
        // require that F only binds to {Field<F>} specializations or derived classes
        []<functions::function_c F>(const Field<F> &) {
        }(c);
    };

    // concept of a field {FIELD} being a scalar field
    template <class FIELD>
    // a scalar field  is a field returning a scalar
    concept scalar_field_c = field_c<FIELD> and tensor::scalar_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a vector field
    template <class FIELD>
    // a vector field  is a field returning a vector
    concept vector_field_c = field_c<FIELD> and tensor::vector_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a tensor field
    template <class FIELD>
    // a tensor field  is a field returning a matrix
    concept tensor_field_c = field_c<FIELD> and tensor::matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a symmetric tensor field
    template <class FIELD>
    // a symmetric tensor field  is a field returning a symmetric matrix
    concept symmetric_tensor_field_c =
        field_c<FIELD> and tensor::symmetric_matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a diagonal tensor field
    template <class FIELD>
    // a diagonal tensor field  is a field returning a diagonal matrix
    concept diagonal_tensor_field_c =
        field_c<FIELD> and tensor::diagonal_matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} of one-forms
    template <class FIELD>
    // a one-form field  is a field returning a one-form
    concept one_form_field_c = field_c<FIELD> and tensor::one_form_c<typename FIELD::output_type>;

    // concept of a field {FIELD} of p-forms
    template <class FIELD>
    // a p-form field  is a field returning a p-form
    concept p_form_field_c = field_c<FIELD> and tensor::p_form_c<typename FIELD::output_type>;

    // concept of two fields being compatible with each other (i.e. defined on the same coordinates)
    template <class FIELD1, class FIELD2>
    // a scalar field  is a field returning a scalar
    concept compatible_fields_c = requires {
        std::is_same_v<typename FIELD1::coordinates_type, typename FIELD2::coordinates_type>;
    };
}


// end of file
