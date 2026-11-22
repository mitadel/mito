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
            const problem_type & problem, const solver_type & solver, scalar_type alpha) :
            _problem(problem),
            _solver(solver),
            _alpha(alpha),
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

            // compute Keff = K + alpha * M
            if constexpr (problemT::time_invariant) {
                _compute_effective_stiffness_matrix();
            }
        }

        // solve (K + alpha * M) * u = f + alpha * M * beta for {u}
        auto solve(scalar_type alpha, const vector_type & beta, vector_type & u) -> void
        {
            bool rebuild = (alpha != _alpha);

            // set the values
            if constexpr (!problemT::time_invariant) {
                _problem.template assemble_stiffness<linalg_backend_type>(_K);
                _problem.template assemble_mass<linalg_backend_type>(_M);
                rebuild = true;
            }
            if constexpr (!problemT::constant_load) {
                _problem.template assemble_load<linalg_backend_type>(_f);
            }

            // rebuild the effective stiffness matrix if alpha has changed
            if (rebuild) {
                // update alpha
                _alpha = alpha;
                // compute Keff = K + alpha * M
                _compute_effective_stiffness_matrix();
            }

            // rhs = M * beta
            _M.mult_vector(beta, _rhs);

            // rhs = f + alpha * rhs
            _rhs.add(_alpha, _f);

            // solve the linear system
            _solver.solve(_Keff, _rhs, u);

            // all done
            return;
        }

        // the problem
        constexpr auto problem() const -> const problem_type & { return _problem; }

      private:
        // compute the effective stiffness matrix K + alpha * M
        auto _compute_effective_stiffness_matrix() -> void
        {
            // compute the effective load vector
            _Keff.copy_values(_K, linalg::sparsity_pattern::same);
            _Keff.add(_alpha, _M, linalg::sparsity_pattern::same);

            // all done
            return;
        }


      private:
        // the problem
        const problem_type & _problem;
        // the solver
        const solver_type & _solver;
        // alpha
        scalar_type _alpha;
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
