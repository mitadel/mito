// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    // enum for sparsity pattern
    enum class sparsity_pattern : std::underlying_type_t<MatStructure> {
        same = SAME_NONZERO_PATTERN,
        subset = SUBSET_NONZERO_PATTERN,
        different = DIFFERENT_NONZERO_PATTERN
    };

    // enum for duplicate policy
    enum class duplicate_policy : std::underlying_type_t<MatDuplicateOption> {
        copy_pattern = MAT_DO_NOT_COPY_VALUES,
        share_pattern = MAT_SHARE_NONZERO_PATTERN
    };

    class Matrix {
      private:
        // the index type
        using index_type = index_t;
        // the scalar type
        using scalar_type = scalar_t;
        // the matrix type
        using matrix_type = Mat;

      public:
        // constructor
        Matrix(index_t);

        // copy constructor
        Matrix(const Matrix &);

        // move constructor
        Matrix(Matrix &&) noexcept;

        // copy assignment operator
        Matrix & operator=(const Matrix &);

        // move assignment operator
        Matrix & operator=(Matrix &&) noexcept;

        // destructor
        ~Matrix();

      public:
        // assemble the matrix
        auto assemble() -> void;

        // assemble the matrix
        auto assemble_flush() -> void;

        // set the value of a matrix entry
        auto insert_value(index_type, index_type, const scalar_type &) -> void;

        // add a value to a matrix entry
        auto add_value(index_type, index_type, const scalar_type &) -> void;

        // get the value of a matrix entry
        auto get_value(index_type, index_type, scalar_type &) const -> void;

        // return the value of a matrix entry
        auto get_value(index_type, index_type) const -> scalar_type;

        // copy structure from another matrix
        auto copy_structure(
            const Matrix & X, duplicate_policy policy = duplicate_policy::copy_pattern) -> void;

        // copy values from another matrix
        auto copy_values(const Matrix & X, sparsity_pattern pattern = sparsity_pattern::different)
            -> void;

        // add scaled matrix using MatAXPY
        auto add(
            const scalar_type & alpha, const Matrix & X,
            sparsity_pattern pattern = sparsity_pattern::different) -> void;

        // get the number of rows/columns
        auto size(index_type &) const -> void;

        // return the number of rows/columns
        auto size() const -> index_type;

        // print the matrix
        auto print() const -> void;

        // get petsc matrix
        auto get() const -> matrix_type const &;

      private:
        // the matrix
        matrix_type _matrix;
    };

}    // namespace mito


// end of file
