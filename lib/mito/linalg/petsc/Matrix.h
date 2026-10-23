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
        // the block segment type
        using block_segment_type = block_segment_t;

      public:
        // constructor
        Matrix(index_t, assembly_format format = assembly_format::csr);

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

        // set pattern
        auto set_pattern(
            std::vector<index_type> rows, std::vector<index_type> cols,
            std::vector<block_segment_type> segments = {}) -> void;

        // set values
        auto set_values(const std::vector<scalar_type> & values) -> void;

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
        // swap the contents of two matrices
        auto _swap(Matrix &) noexcept -> void;

        // set the pattern based on the provided row and column indices, for hash assembly
        auto _set_pattern_hash(std::vector<index_type> rows, std::vector<index_type> cols) -> void;

        // set the pattern based on the provided row and column indices, for coo assembly
        auto _set_pattern_coo(std::vector<index_type> rows, std::vector<index_type> cols) -> void;

        // set the pattern based on the provided row and column indices, for csr assembly
        auto _set_pattern_csr(std::vector<index_type> rows, std::vector<index_type> cols) -> void;

        // set the values for coo assembly
        auto _set_values_coo(const std::vector<scalar_type> & values) -> void;

        // set the values for csr assembly
        auto _set_values_csr(const std::vector<scalar_type> & values) -> void;

      private:
        // the matrix
        matrix_type _matrix;
        // the assembly format
        assembly_format _format;
        // the number of COO entries (only used for COO assembly)
        std::size_t _n_entries;
        // the index lists (only used for CSR assembly)
        std::vector<index_type> _rows;
        std::vector<index_type> _cols;
        // the block segments (only used for CSR assembly)
        std::vector<block_segment_type> _segments;
        // flag to indicate if the pattern has been set
        bool _pattern_set = false;
    };

}    // namespace mito


// end of file
