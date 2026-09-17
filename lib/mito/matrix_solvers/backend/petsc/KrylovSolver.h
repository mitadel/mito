// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


#include "../../../math/backend/petsc.h"


namespace mito::matrix_solvers::petsc {

    class KrylovSolver {
      private:
        // the backend type
        using math_backend_type = typename mito::math_backend::petsc::backend_t;
        // the solver type
        using solver_type = KSP;
        // the options type
        using options_type = std::string;

      public:
        // constructor
        KrylovSolver(typename math_backend_type::matrix_type &, const options_type & = "");

        // destructor
        ~KrylovSolver();

      public:
        // set petsc options
        auto set_options(const options_type &) -> void;

        // // solve the linear system
        // auto solve() -> void;

        // // print the linear system of equations of the petsc solver
        // auto print() const -> void;

      private:
        // the linear system
        typename math_backend_type::matrix_type & _matrix;
        // the prefix for the PETSc options
        options_type _options_prefix;
        // the Krylov solver
        solver_type _ksp;
    };

}    // namespace mito


// end of file
