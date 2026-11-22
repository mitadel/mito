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

      private:
        // the coefficients of Alexander's 3-stage, 3rd-order, L-stable SDIRK scheme
        static constexpr scalar_type gamma = alexander_gamma();
        static constexpr scalar_type a21 = (1. - gamma) / 2.;
        static constexpr scalar_type a31 = -(6. * gamma * gamma - 16. * gamma + 1.) / 4.;
        static constexpr scalar_type a32 = (6. * gamma * gamma - 20. * gamma + 5.) / 4.;

        // the stage coefficients: the right-hand side of stage i is M/dt * sum_j beta_ij U_j
        static constexpr scalar_type beta21 = a21 / (gamma * gamma);
        static constexpr scalar_type beta20 = 1. / gamma - beta21;
        static constexpr scalar_type beta32 = a32 / (gamma * gamma);
        static constexpr scalar_type beta31 = (a31 - a32 * a21 / gamma) / (gamma * gamma);
        static constexpr scalar_type beta30 = 1. / gamma - beta31 - beta32;

      public:
        // the default constructor
        constexpr SDIRK3S3O(
            const problem_type & problem, solver_type && solver,
            const fields::field_c auto & initial_condition, scalar_type dt) :
            _problem(problem),
            _solver(std::move(solver)),
            _dt(dt),
            _K("stiffness", _problem.n_equations()),
            _M("capacitance", _problem.n_equations()),
            _f("load", _problem.n_equations()),
            _Keff("effective_stiffness", _problem.n_equations()),
            _rhs("rhs", _problem.n_equations()),
            _u("solution",
               _problem.template transform_field_to_vector<linalg_backend_type>(initial_condition)),
            _U1("stage_vector_1", _problem.n_equations()),
            _U2("stage_vector_2", _problem.n_equations()),
            _w("work_vector", _problem.n_equations())
        {
            ;
        }

        constexpr auto initialise() -> void
        {
            // set the patterns
            _problem.template set_pattern<linalg_backend_type>(_K);
            _problem.template set_pattern<linalg_backend_type>(_M);

            // set the values
            _problem.template assemble_stiffness<linalg_backend_type>(_K);
            _problem.template assemble_mass<linalg_backend_type>(_M);
            _problem.template assemble_load<linalg_backend_type>(_f);

            // initialise the effective stiffness matrix by duplicating the stiffness matrix pattern
            _Keff.duplicate(_K);

            // compute the effective stiffness matrix for the provided time step size
            compute_effective_stiffness_matrix();
        }

        // set the time step size
        constexpr auto set_dt(scalar_type dt) -> void
        {
            // set the time step size
            _dt = dt;

            // reassemble the effective stiffness matrix with the new time step size
            compute_effective_stiffness_matrix();

            // all done
            return;
        }

        // march one time step
        constexpr auto march() -> void
        {
            // stage 1: w = u_n / (gamma dt); rhs = f + M * w; Keff * U1 = rhs
            _w.zero();
            _w.add(std::array{ 1. / (gamma * _dt) }, std::array{ &_u });
            _M.mult_add_vector(_w, _f, _rhs);
            _solver.solve(_Keff, _rhs, _U1);

            // stage 2: w = (beta20 u_n + beta21 U1) / dt; rhs = f + M * w; Keff * U2 = rhs
            _w.zero();
            _w.add(std::array{ beta20 / _dt, beta21 / _dt }, std::array{ &_u, &_U1 });
            _M.mult_add_vector(_w, _f, _rhs);
            _solver.solve(_Keff, _rhs, _U2);

            // stage 3: w = (beta30 u_n + beta31 U1 + beta32 U2) / dt; rhs = f + M * w; Keff *
            // u_{n+1} = rhs
            _w.zero();
            _w.add(
                std::array{ beta30 / _dt, beta31 / _dt, beta32 / _dt },
                std::array{ &_u, &_U1, &_U2 });
            _M.mult_add_vector(_w, _f, _rhs);
            _solver.solve(_Keff, _rhs, _u);

            // all done
            return;
        }

        // the solution vector
        constexpr auto solution() const -> const vector_type & { return _u; }

      private:
        // compute effective stiffness matrix
        constexpr auto compute_effective_stiffness_matrix() -> void
        {
            // copy the stiffness matrix
            _Keff.copy_values(_K, linalg::sparsity_pattern::same);

            // add the capacitance matrix scaled by 1/(gamma*dt) to the effective stiffness matrix
            _Keff.add(1. / (gamma * _dt), _M, linalg::sparsity_pattern::same);

            // all done
            return;
        };


      private:
        // the discrete system
        const problem_type & _problem;
        // the solver
        solver_type _solver;
        // the time step size
        scalar_type _dt;
        // the stiffness matrix, stores K
        matrix_type _K;
        // the capacitance matrix, stores M
        matrix_type _M;
        // the load vector, stores f
        vector_type _f;
        // the effective stiffness matrix, stores Keff = K + 1/(gamma*dt) * M
        matrix_type _Keff;
        // the right-hand side vector, stores rhs = f + M * w
        vector_type _rhs;
        // the solution vector, stores u_n
        vector_type _u;
        // the stage vectors, stores U1 and U2
        vector_type _U1;
        vector_type _U2;
        // the work vector, stores a linear combination of u_n, U1, and U2 based on the stage
        vector_type _w;
    };


}    // namespace mito::timesteppers


// end of file
