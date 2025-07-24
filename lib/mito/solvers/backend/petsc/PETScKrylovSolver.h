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
        // the linear system type
        using linear_system_type = PETScLinearSystem;
        // the solver type
        using solver_type = KSP;
        // the options type
        using options_type = std::string;

      public:
        // constructor
        PETScKrylovSolver(linear_system_type &);

        // destructor
        ~PETScKrylovSolver();

      public:
        // create the Krylov solver
        auto create() -> void;

        // destroy the Krylov solver
        auto destroy() -> void;

        // set petsc options
        auto set_options(const options_type &) -> void;

        // solve the linear system
        auto solve() -> void;

        // print the linear system of equations of the petsc solver
        auto print() const -> void;

      private:
        // the linear system
        linear_system_type & _linear_system;
        // the prefix for the PETSc options
        options_type _options_prefix;
        // the Krylov solver
        solver_type _ksp;
    };

}    // namespace mito


// end of file
