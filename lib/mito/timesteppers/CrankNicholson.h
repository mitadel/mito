// -*- c++ -*-
//
// Copyright (c) 2020-2026, the MiTo Authors, all rights reserved
//

// code guard
#pragma once


namespace mito::timesteppers {

    template <
        linalg::valid_backend_c linalgBackendT, class problemT, class solverT,
        template <class, class, class> class stageSolverT>
    class CrankNicholson {

      private:
        // the linear algebra backend type
        using linalg_backend_type = linalgBackendT;
        // the problem type
        using problem_type = problemT;
        // the matrix solver type
        using solver_type = solverT;
        // the vector type
        using vector_type = typename linalg_backend_type::vector_type;
        // the scalar type
        using scalar_type = typename linalg_backend_type::scalar_type;
        // the stage solver type
        using stage_solver_type = stageSolverT<linalg_backend_type, problem_type, solver_type>;

      public:
        // the default constructor
        constexpr CrankNicholson(
            const problem_type & problem, solver_type & solver,
            const vector_type & initial_condition, scalar_type dt) :
            _stage_solver(problem, solver, 2.0 / dt),
            _dt(dt),
            _beta("predictor", problem.n_equations()),
            _u_dot("rate", problem.n_equations()),
            _u(initial_condition)
        {
            ;
        }

        // set the time step size
        constexpr auto set_dt(scalar_type dt) -> void
        {
            // set the time step size
            _dt = dt;

            // all done
            return;
        }

        // march one time step
        constexpr auto march() -> void
        {
            // for the first time step, apply implicit Euler
            if (!_has_rate) {
                // apply implicit Euler
                apply_implicit_euler();

                // all done
                return;
            }

            // the factor alpha = 2 / dt
            const scalar_type alpha = 2.0 / _dt;

            // the predictor beta = u_n + dt/2 * u_dot_n
            _beta.copy_values(_u);
            _beta.add(_dt * 0.5, _u_dot);

            // solve the linear system
            _stage_solver.solve(alpha, _beta, _u);

            // the stage rate at u_{n+1}: u_dot_{n+1} = alpha * (u_{n+1} - beta)
            stage_rate(alpha, _beta, _u, _u_dot);

            // all done
            return;
        }

        // the problem
        constexpr auto problem() const -> const problem_type & { return _stage_solver.problem(); }

        // the solution vector
        constexpr auto solution() const -> const vector_type & { return _u; }

      private:
        constexpr auto apply_implicit_euler() -> void
        {
            // the factor alpha = 1 / dt
            const scalar_type alpha = 1.0 / _dt;

            // the predictor beta = u_n
            _beta.copy_values(_u);

            // solve the linear system
            _stage_solver.solve(alpha, _beta, _u);

            // the stage rate at u_{n+1}: u_dot_{n+1} = alpha * (u_{n+1} - beta)
            stage_rate(alpha, _u, _beta, _u_dot);

            // the rate at the current solution is known
            _has_rate = true;

            // all done
            return;
        }

      private:
        // the stage solver
        stage_solver_type _stage_solver;
        // the time step size
        scalar_type _dt;
        // whether the rate at the current solution is known
        bool _has_rate = false;
        // the predictor vector
        vector_type _beta;
        // the rate du/dt at the current solution
        vector_type _u_dot;
        // the solution vector
        vector_type _u;
    };


}    // namespace mito::timesteppers


// end of file
