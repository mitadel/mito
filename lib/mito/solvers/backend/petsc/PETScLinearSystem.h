// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::petsc {

    class PETScLinearSystem {

        // friend declarations
        friend class PETScKrylovSolver;

      private:
        // the index type
        using index_type = PetscInt;
        // the scalar type
        using scalar_type = PetscScalar;
        // the vector type
        using vector_type = Vec;
        // the matrix type
        using matrix_type = Mat;
        // the label type
        using label_type = std::string;

      public:
        // constructor
        PETScLinearSystem(const label_type &);

        // destructor
        ~PETScLinearSystem();

      public:
        // create the matrix, right-hand side, and solution
        auto create(index_type) -> void;

        // destroy the matrix, right-hand side, and solution
        auto destroy() -> void;

        // get the label of the linear system
        auto label() const -> label_type;

        // assemble the linear system
        auto assemble() -> void;

        // set the value of a matrix entry
        auto insert_matrix_value(index_type, index_type, const scalar_type &) -> void;

        // add a value to a matrix entry
        auto add_matrix_value(index_type, index_type, const scalar_type &) -> void;

        // set the value of a right-hand side entry
        auto insert_rhs_value(index_type, const scalar_type &) -> void;

        // add a value to a right-hand side entry
        auto add_rhs_value(index_type, const scalar_type &) -> void;

        // get the solution vector
        template <class solutionT>
        auto get_solution(solutionT & solution) const -> void;

        // print the linear system
        auto print() const -> void;

      private:
        // a flag to recall if this instance has initialized PETSc
        bool _initialized_petsc;
        // the label for the linear system (this is used to prefix PETSc options)
        label_type _label;
        // the matrix
        matrix_type _matrix;
        // the right-hand side vector
        vector_type _rhs;
        // the solution vector
        vector_type _solution;
    };

}    // namespace mito


// get the template definitions
#define mito_solvers_backend_petsc_PETScLinearSystem_icc
#include "PETScLinearSystem.icc"
#undef mito_solvers_backend_petsc_PETScLinearSystem_icc


// end of file
