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
        auto size() const -> int;

        // print the matrix
        auto print() const -> void;

        // access matrix
        auto matrix() -> matrix_type &;

      private:
        // a flag to recall if this instance has initialized PETSc
        bool _initialized_petsc;
        // the label for the matrix (this is used to prefix PETSc options)
        label_type _label;
        // the matrix
        matrix_type _matrix;
        // the number of rows/columns
        int _size;
    };

}    // namespace mito


// end of file
