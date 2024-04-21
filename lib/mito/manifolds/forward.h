// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#if !defined(mito_manifolds_forward_h)
#define mito_manifolds_forward_h


namespace mito::manifolds {

    // class manifold
    template <class cellT, geometry::CoordinateType coordsT>
    class Manifold;

    // class P-form
    template <int P, class F>
    class Form;

    // class field
    template <geometry::CoordinateType coordT, functions::function_c F>
    requires(geometry::coordinates_c<typename F::input_type>)
    class Field;

    // concept of a field
    template <class FIELD>
    concept field_c = requires(FIELD c) {
        // require that F only binds to {Field<coordT, F>} specializations or derived classes
        []<geometry::CoordinateType coordT, functions::function_c F>(const Field<coordT, F> &) {
        }(c);
    };

    // concept of a field {FIELD} being a scalar field on a D-dimensional manifold
    template <class FIELD>
    // a scalar field on a D-dimensional manifold is a field returning a scalar
    concept scalar_field_c = field_c<FIELD> and scalar_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a vector field on a D-dimensional manifold
    template <class FIELD>
    // a vector field on a D-dimensional manifold is a field returning a vector
    concept vector_field_c = field_c<FIELD> and vector_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a tensor field on a D-dimensional manifold
    template <class FIELD>
    // a tensor field on a D-dimensional manifold is a field returning a matrix
    concept tensor_field_c = field_c<FIELD> and matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a symmetric tensor field on a D-dimensional manifold
    template <class FIELD>
    // a symmetric tensor field on a D-dimensional manifold is a field returning a symmetric matrix
    concept symmetric_tensor_field_c =
        field_c<FIELD> and symmetric_matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a diagonal tensor field on a D-dimensional manifold
    template <class FIELD>
    // a diagonal tensor field on a D-dimensional manifold is a field returning a diagonal matrix
    concept diagonal_tensor_field_c =
        field_c<FIELD> and diagonal_matrix_c<typename FIELD::output_type>;

    // concept of a one-form
    template <class F>
    concept one_form_c = requires(F c) {
        // require that F only binds to {one_form_t} specializations
        []<class T>(const Form<1, T> &) {
        }(c);
    };

    // concept of a field {FIELD} of one-forms
    template <class FIELD>
    // a one-form field on a D-dimensional manifold is a field returning a one-form
    concept one_form_field_c = field_c<FIELD> and one_form_c<typename FIELD::output_type>;

    // concept of a p-form
    template <class F>
    concept p_form_c = requires(F c) {
        // require that F only binds to {p_form_t} specializations
        []<int P, class T>(const Form<P, T> &) {
        }(c);
    };

    // concept of a field {FIELD} of p-forms
    template <class FIELD>
    // a p-form field on a D-dimensional manifold is a field returning a p-form
    concept p_form_field_c = field_c<FIELD> and p_form_c<typename FIELD::output_type>;
}


#endif

// end of file
