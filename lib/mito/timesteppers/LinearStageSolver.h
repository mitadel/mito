// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    template <
        linalg::valid_backend_c linalgBackendT,
        linear_first_order_problem_c<linalgBackendT> problemT, class solverT>
    class LinearStageSolver {

      private:
        // the linear algebra backend type
        using linalg_backend_type = linalgBackendT;
        // the problem type
        using problem_type = problemT;
        // the matrix solver type
        using solver_type = solverT;
        // the matrix type
        using matrix_type = typename linalg_backend_type::matrix_type;
        // the vector type
        using vector_type = typename linalg_backend_type::vector_type;
        // the scalar type
        using scalar_type = typename linalg_backend_type::scalar_type;

      public:
        // the default constructor
        constexpr LinearStageSolver(
            const problem_type & problem, const solver_type & solver, scalar_type shift) :
            _solver(solver),
            _shift(shift),
            _K("stiffness", _problem.n_equations()),
            _M("capacitance", _problem.n_equations()),
            _f("load", _problem.n_equations()),
            _Keff("effective_stiffness", _problem.n_equations()),
            _rhs("effective_load", _problem.n_equations())
        {
            // set the patterns
            _problem.template set_pattern<linalg_backend_type>(_K);
            _problem.template set_pattern<linalg_backend_type>(_M);

            // set the values
            if constexpr (problemT::time_invariant) {
                _problem.template assemble_stiffness<linalg_backend_type>(_K);
                _problem.template assemble_mass<linalg_backend_type>(_M);
            }
            if constexpr (problemT::constant_load) {
                _problem.template assemble_load<linalg_backend_type>(_f);
            }

            // initialise the effective stiffness matrix by duplicating the stiffness matrix pattern
            _Keff.duplicate(_K);

            // compute Keff = K + shift * M
            _Keff.copy_values(_K, linalg::sparsity_pattern::same);
            _Keff.add(_shift, _M, linalg::sparsity_pattern::same);
        }

        // solve (K + a * M) * u = f + a * M * w for {u}
        auto solve(scalar_type a, const vector_type & w, vector_type & u) -> void
        {
            bool rebuild = (a != _shift);

            // set the values
            if constexpr (!problemT::time_invariant) {
                _problem.template assemble_stiffness<linalg_backend_type>(_K);
                _problem.template assemble_mass<linalg_backend_type>(_M);
                rebuild = true;
            }
            if constexpr (!problemT::constant_load) {
                _problem.template assemble_load<linalg_backend_type>(_f);
            }

            // rebuild the effective stiffness matrix if the shift has changed
            if (rebuild) {
                // update the shift
                _shift = a;
                // compute Keff = K + shift * M
                _Keff.copy_values(_K, linalg::sparsity_pattern::same);
                _Keff.add(_shift, _M, linalg::sparsity_pattern::same);
            }

            // rhs = M * w
            _M.mult_vector(w, _rhs);
            // rhs = f + shift * rhs
            _rhs.add(_shift, _f);

            // solve the linear system
            _solver.solve(_Keff, _rhs, u);

            // all done
            return;
        }

      private:
        // compute the effective stiffness matrix a
        auto _compute_effective_stiffness_matrix(const vector_type & w) -> void
        {
            // compute the effective load vector
            _Keff.copy_values(_K, linalg::sparsity_pattern::same);
            _Keff.add(_shift, _M, linalg::sparsity_pattern::same);

            // all done
            return;
        }


      private:
        // the solver
        const solver_type & _solver;
        // the shift
        scalar_type _shift;
        // the stiffness matrix
        matrix_type _K;
        // the capacitance matrix
        matrix_type _M;
        // the load vector
        vector_type _f;
        // the effective stiffness matrix K + a M
        matrix_type _Keff;
        // the effective load vector
        vector_type _rhs;
    };


}    // namespace mito::timesteppers


// end of file
