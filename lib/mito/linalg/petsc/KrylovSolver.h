// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::linalg::petsc {

    class KrylovSolver {
      private:
        // the solver type
        using solver_type = KSP;
        // the options type
        using options_type = std::string;

      public:
        // constructor
        KrylovSolver(std::string name = "krylov");

        // copy constructor is deleted because PETSc cannot duplicate a Krylov solver; copying the
        // handle would lead to double destruction of the same Krylov solver
        KrylovSolver(const KrylovSolver &) = delete;

        // move constructor
        KrylovSolver(KrylovSolver &&) noexcept;

        // copy assignment operator is deleted because PETSc cannot duplicate a Krylov solver;
        // copying the handle would lead to double destruction of the same Krylov solver
        KrylovSolver & operator=(const KrylovSolver &) = delete;

        // move assignment operator
        KrylovSolver & operator=(KrylovSolver &&) noexcept;

        // destructor
        ~KrylovSolver();

      public:
        // set petsc options
        auto set_options(const options_type &) -> void;

        // solve A x = b
        auto solve(const matrix_t & A, const vector_t & b, vector_t & x) -> void;

        // print the linear system of equations of the petsc solver
        auto print() const -> void;

      private:
        // the prefix for the PETSc options
        options_type _options_prefix;
        // the Krylov solver
        solver_type _ksp;
    };

}    // namespace mito


// end of file
