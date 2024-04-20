// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// TOFIX: rename into traits?
// code guard
#if !defined(mito_manifold_utilities_h)
#define mito_manifold_utilities_h


namespace mito::manifolds {

    // TOFIX: implement name consistency for concepts (e.g. scalar_field_c)

    // concept of a field {FIELD} being a scalar field on a D-dimensional manifold
    template <class FIELD>
    // a scalar field on a D-dimensional manifold is a field returning a scalar
    concept ScalarField = scalar_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a vector field on a D-dimensional manifold
    template <class FIELD>
    // a vector field on a D-dimensional manifold is a field returning a vector
    concept VectorField = vector_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a tensor field on a D-dimensional manifold
    template <class FIELD>
    // a tensor field on a D-dimensional manifold is a field returning a matrix
    concept TensorField = matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a symmetric tensor field on a D-dimensional manifold
    template <class FIELD>
    // a symmetric tensor field on a D-dimensional manifold is a field returning a symmetric matrix
    concept SymmetricTensorField = symmetric_matrix_c<typename FIELD::output_type>;

    // concept of a field {FIELD} being a diagonal tensor field on a D-dimensional manifold
    template <class FIELD>
    // a diagonal tensor field on a D-dimensional manifold is a field returning a diagonal matrix
    concept DiagonalTensorField = diagonal_matrix_c<typename FIELD::output_type>;
}


#endif

// end of file
