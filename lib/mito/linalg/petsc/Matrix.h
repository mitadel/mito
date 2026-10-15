// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

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
        Matrix(const Matrix &) = delete;

        // move constructor
        Matrix(Matrix &&) = delete;

        // copy assignment operator
        Matrix & operator=(const Matrix &) = delete;

        // move assignment operator
        Matrix & operator=(Matrix &&) = delete;

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
