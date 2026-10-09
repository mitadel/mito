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
        using vector_type = Vec;
        // the matrix type
        using matrix_type = Mat;
        // the label type
        using label_type = std::string;

      public:
        // constructor
        LinearSystem(const label_type &, index_t);

        // copy constructor
        LinearSystem(const LinearSystem &) = delete;

        // move constructor
        LinearSystem(LinearSystem &&) = delete;

        // copy assignment operator
        LinearSystem & operator=(const LinearSystem &) = delete;

        // move assignment operator
        LinearSystem & operator=(LinearSystem &&) = delete;

        // destructor
        ~LinearSystem();

      public:
        // get the label of the linear system
        auto label() const -> label_type;

        // assemble the linear system
        auto assemble() -> void;

        // set the value of a matrix entry
        auto insert_matrix_value(index_t, index_t, const scalar_t &) -> void;

        // add a value to a matrix entry
        auto add_matrix_value(index_t, index_t, const scalar_t &) -> void;

        // set the value of a right-hand side entry
        auto insert_rhs_value(index_t, const scalar_t &) -> void;

        // add a value to a right-hand side entry
        auto add_rhs_value(index_t, const scalar_t &) -> void;

        // accessor to the number of equations
        auto n_equations() const -> index_t;

        // get the solution vector
        template <class solutionT>
        auto get_solution(solutionT & solution) const -> void;

        // print the linear system
        auto print() const -> void;

      private:
        // the label for the linear system (this is used to prefix PETSc options)
        label_type _label;
        // the matrix
        matrix_type _matrix;
        // the right-hand side vector
        vector_type _rhs;
        // the solution vector
        vector_type _solution;
        // the number of equations
        index_t _n_equations;
    };

}    // namespace mito


// get the template definitions
#define mito_linalg_backend_petsc_LinearSystem_icc
#include "LinearSystem.icc"
#undef mito_linalg_backend_petsc_LinearSystem_icc


// end of file
