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
    class SDIRK3S3O {

      private:
        // the problem type
        using problem_type = problemT;
        // the linear algebra backend type
        using linalg_backend_type = linalgBackendT;
        // the matrix solver type
        using solver_type = solverT;
        // the matrix type
        using matrix_type = typename linalg_backend_type::matrix_type;
        // the vector type
        using vector_type = typename linalg_backend_type::vector_type;
        // the scalar type
        using scalar_type = typename linalg_backend_type::scalar_type;
        // the stage solver type
        using stage_solver_type = stageSolverT<linalg_backend_type, problem_type, solver_type>;

      private:
        // the coefficients of Alexander's 3-stage, 3rd-order, L-stable SDIRK scheme
        static constexpr scalar_type gamma = alexander_gamma();
        static constexpr scalar_type a21 = (1. - gamma) / 2.;
        static constexpr scalar_type a31 = -(6. * gamma * gamma - 16. * gamma + 1.) / 4.;
        static constexpr scalar_type a32 = (6. * gamma * gamma - 20. * gamma + 5.) / 4.;

      public:
        // the default constructor
        constexpr SDIRK3S3O(
            const problem_type & problem, solver_type & solver,
            const vector_type & initial_condition, scalar_type dt) :
            _stage_solver(problem, solver, 1.0 / (gamma * dt)),
            _dt(dt),
            _beta("predictor", problem.n_equations()),
            _U("stage_value", problem.n_equations()),
            _U1_dot("stage_rate_1", problem.n_equations()),
            _U2_dot("stage_rate_2", problem.n_equations()),
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
            // alpha, the same in all stages
            const scalar_type alpha = 1.0 / (gamma * _dt);

            // stage 1: beta_1 = u_n
            _beta.copy_values(_u);
            _stage_solver.solve(alpha, _beta, _U);
            stage_rate(alpha, _beta, _U, _U1_dot);

            // stage 2: beta_2 = u_n + dt * (a21 * U1_dot);
            _beta.copy_values(_u);
            _beta.add(_dt * a21, _U1_dot);
            _stage_solver.solve(alpha, _beta, _U);
            stage_rate(alpha, _beta, _U, _U2_dot);

            // stage 3: beta_3 = u_n + dt * (a31 * U1_dot + a32 * U2_dot);
            _beta.copy_values(_u);
            _beta.add(_dt * a31, _U1_dot);
            _beta.add(_dt * a32, _U2_dot);
            _stage_solver.solve(alpha, _beta, _u);

            // all done
            return;
        }

        // the problem
        constexpr auto problem() const -> const problem_type & { return _stage_solver.problem(); }

        // the solution vector
        constexpr auto solution() const -> const vector_type & { return _u; }

      private:
        // the stage solver
        stage_solver_type _stage_solver;
        // the time step size
        scalar_type _dt;
        // the predictor vector
        vector_type _beta;
        // the stage value of stages 1 and 2
        vector_type _U;
        // the stage rates of stages 1 and 2
        vector_type _U1_dot;
        vector_type _U2_dot;
        // the solution vector
        vector_type _u;
    };


}    // namespace mito::timesteppers


// end of file
