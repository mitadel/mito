// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::math_backend::petsc {

    class Matrix {
      public:
        // the index type
        using index_type = PetscInt;
        // the scalar type
        using scalar_type = PetscScalar;

      private:
        // the matrix type
        using matrix_type = Mat;
        // the label type
        using label_type = std::string;

      public:
        // constructor
        Matrix(const label_type &, index_type);

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
        auto insert_value(index_type, index_type, const scalar_type &) -> void;

        // add a value to a matrix entry
        auto add_value(index_type, index_type, const scalar_type &) -> void;

        // get the value of a matrix entry
        auto get_value(index_type, index_type, scalar_type &) const -> void;

        // return the value of a matrix entry
        auto get_value(index_type, index_type) const -> scalar_type;

        // accessor to the number of equations
        auto n_equations() const -> int;

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
        // the number of equations
        int _n_equations;
    };

}    // namespace mito


// end of file
