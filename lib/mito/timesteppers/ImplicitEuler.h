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
    class ImplicitEuler {

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
        // the stage solver type, built from the types of this time stepper
        using stage_solver_type = stageSolverT<linalg_backend_type, problem_type, solver_type>;

      public:
        // the default constructor
        constexpr ImplicitEuler(
            const problemT & problem, solver_type & solver,
            const vector_type & initial_condition, scalar_type dt) :
            _stage_solver(problem, solver, 1.0 / dt),
            _dt(dt),
            _beta("predictor", problem.n_equations()),
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
            // the predictor beta = u_n
            _beta.copy_values(_u);

            // the stage solve, with alpha = 1.0/dt and beta = u_n, into u_{n+1}
            _stage_solver.solve(1.0 / _dt, _beta, _u);

            // all done
            return;
        }

        // the problem
        constexpr auto problem() const -> const problem_type & { return _stage_solver.problem(); }

        // the solution vector
        constexpr auto solution() const -> const vector_type & { return _u; }


      private:
        // the solver
        stage_solver_type _stage_solver;
        // the time step size
        scalar_type _dt;
        // the predictor vector
        vector_type _beta;
        // the solution vector
        vector_type _u;
    };


}    // namespace mito::timesteppers


// end of file
