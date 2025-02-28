// -*- c++ -*-
//
// Copyright (c) 2020-2024, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::solvers::petsc {

    class PETScKrylovSolver {
      private:
        // the index type
        using index_type = PetscInt;
        // the scalar type
        using scalar_type = PetscScalar;
        // the vector type
        using vector_type = Vec;
        // the matrix type
        using matrix_type = Mat;
        // the solver type
        using solver_type = KSP;
        // the options type
        using options_type = std::string;
        // the label type
        using label_type = std::string;

      public:
        // constructor
        PETScKrylovSolver(const label_type &);

        // destructor
        ~PETScKrylovSolver();

      private:
        // initialize PETSc if it has not been initialized before
        auto _initialize_petsc() -> void;

        // finalize PETSc if it has been initialized by this instance
        auto _finalize_petsc() -> void;

        // create the matrix, right-hand side, solution, and Krylov solver
        auto _create_linear_system(index_type) -> void;

        // destroy the matrix, right-hand side, solution, and Krylov solver
        auto _destroy_linear_system() -> void;

      public:
        // initialize the petsc solver
        auto initialize(index_type size) -> void;

        // finalize the petsc solver
        auto finalize() -> void;

        // set petsc options
        auto set_options(const options_type &) -> void;

        // set the value of a matrix entry
        auto insert_matrix_value(index_type, index_type, const scalar_type &) -> void;

        // add a value to a matrix entry
        auto add_matrix_value(index_type, index_type, const scalar_type &) -> void;

        // set the value of a right-hand side entry
        auto insert_rhs_value(index_type, const scalar_type &) -> void;

        // add a value to a right-hand side entry
        auto add_rhs_value(index_type, const scalar_type &) -> void;

        // solve the linear system
        auto solve() -> void;

        // get the solution vector
        template <class solutionT>
        auto get_solution(solutionT & solution) const -> void;

      private:
        // the prefix for the PETSc options
        label_type _options_prefix;
        // a flag to recall if this instance has initialized PETSc
        bool _initialized_petsc;
        // the matrix
        matrix_type _matrix;
        // the right-hand side vector
        vector_type _rhs;
        // the solution vector
        vector_type _solution;
        // the Krylov solver
        solver_type _ksp;
    };

}    // namespace mito


// get the template definitions
#define mito_solvers_backend_petsc_PETScKrylovSolver_icc
#include "PETScKrylovSolver.icc"
#undef mito_solvers_backend_petsc_PETScKrylovSolver_icc


// end of file
