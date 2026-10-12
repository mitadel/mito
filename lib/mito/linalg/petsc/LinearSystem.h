// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    class LinearSystem {

        // friend declarations
        friend class KrylovSolver;

      private:
        // the vector type
        using vector_type = Vector;
        // the matrix type
        using matrix_type = Matrix;

      public:
        // constructor
        LinearSystem(index_t);

        // copy constructor
        LinearSystem(const LinearSystem &) = delete;

        // move constructor
        LinearSystem(LinearSystem &&) = delete;

        // copy assignment operator
        LinearSystem & operator=(const LinearSystem &) = delete;

        // move assignment operator
        LinearSystem & operator=(LinearSystem &&) = delete;

        // destructor
        ~LinearSystem() = default;

      public:
        // assemble the linear system
        auto assemble() -> void;

        // assemble the linear system
        auto assemble_flush() -> void;

        // set the value of a matrix entry
        auto insert_matrix_value(index_t, index_t, const scalar_t &) -> void;

        // add a value to a matrix entry
        auto add_matrix_value(index_t, index_t, const scalar_t &) -> void;

        // set the value of a right-hand side entry
        auto insert_rhs_value(index_t, const scalar_t &) -> void;

        // add a value to a right-hand side entry
        auto add_rhs_value(index_t, const scalar_t &) -> void;

        // return the number of equations in the linear system
        auto n_equations() const -> index_t;

        // read the solution vector
        template <class solutionT>
        auto read_solution(solutionT & solution) const -> void;

        // print the linear system

        auto print() const -> void;
        // get the matrix
        auto get_matrix() const -> matrix_type const &;

        // get the right-hand side vector
        auto get_rhs() const -> vector_type const &;

        // get the solution vector
        auto get_solution() const -> vector_type const &;


      private:
        // the matrix
        matrix_type _matrix;
        // the right-hand side vector
        vector_type _rhs;
        // the solution vector
        vector_type _solution;
    };

}    // namespace mito


// get the template definitions
#define mito_linalg_backend_petsc_LinearSystem_icc
#include "LinearSystem.icc"
#undef mito_linalg_backend_petsc_LinearSystem_icc


// end of file
