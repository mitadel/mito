// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    class Matrix {
      private:
        // the matrix type
        using matrix_type = Mat;
        // the label type
        using label_type = std::string;

      public:
        // constructor
        Matrix(const label_type &, index_t);

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
        // get the label of the matrix
        auto label() const -> label_type;

        // assemble the matrix
        auto assemble() -> void;

        // assemble the matrix
        auto assembleFlush() -> void;

        // set the value of a matrix entry
        auto insert_value(index_t, index_t, const scalar_t &) -> void;

        // add a value to a matrix entry
        auto add_value(index_t, index_t, const scalar_t &) -> void;

        // get the value of a matrix entry
        auto get_value(index_t, index_t, scalar_t &) const -> void;

        // return the value of a matrix entry
        auto get_value(index_t, index_t) const -> scalar_t;

        // accessor to number of rows/columns
        auto size() const -> index_t;

        // print the matrix
        auto print() const -> void;

        // access matrix
        auto matrix() const -> const matrix_type &;

      private:
        // the label for the matrix (this is used to prefix PETSc options)
        label_type _label;
        // the matrix
        matrix_type _matrix;
        // the number of rows/columns
        index_t _size;
    };

}    // namespace mito


// end of file
