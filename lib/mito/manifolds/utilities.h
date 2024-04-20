// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// TOFIX: rename into traits?
// code guard
#if !defined(mito_manifold_utilities_h)
#define mito_manifold_utilities_h


namespace mito::manifolds {

    // concept of a field {FIELD} being a scalar field on a D-dimensional manifold
    template <class FIELD>
    // a scalar field on a D-dimensional manifold is a field returning a scalar
    concept scalar_field_c = scalar_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a vector field on a D-dimensional manifold
    template <class FIELD>
    // a vector field on a D-dimensional manifold is a field returning a vector
    concept vector_field_c = vector_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a tensor field on a D-dimensional manifold
    template <class FIELD>
    // a tensor field on a D-dimensional manifold is a field returning a matrix
    concept tensor_field_c = matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a symmetric tensor field on a D-dimensional manifold
    template <class FIELD>
    // a symmetric tensor field on a D-dimensional manifold is a field returning a symmetric matrix
    concept symmetric_tensor_field_c = symmetric_matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a diagonal tensor field on a D-dimensional manifold
    template <class FIELD>
    // a diagonal tensor field on a D-dimensional manifold is a field returning a diagonal matrix
    concept diagonal_tensor_field_c = diagonal_matrix_c<typename FIELD::output_type>;
}


#endif

// end of file
