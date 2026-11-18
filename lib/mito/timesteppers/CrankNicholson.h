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
    class CrankNicholson {

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

      public:
        // the default constructor
        constexpr CrankNicholson(
            const problemT & problem, solver_type && solver,
            const fields::field_c auto & initial_condition, scalar_type dt) :
            _problem(problem),
            _solver(std::move(solver)),
            _dt(dt),
            _K("stiffness", _problem.n_equations()),
            _M("capacitance", _problem.n_equations()),
            _f("load", _problem.n_equations()),
            _Keff("effective_stiffness", _problem.n_equations()),
            _B("effective_rhs", _problem.n_equations()),
            _feff("effective_load", _problem.n_equations()),
            _u("solution",
               _problem.template transform_field_to_vector<linalg_backend_type>(initial_condition))
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

            // duplicate the stiffness matrix pattern
            _Keff.duplicate(_K);

            // duplicate the stiffness matrix pattern
            _B.duplicate(_K);

            // compute the effective stiffness matrix and load vector
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
            // compute the effective load vector
            compute_effective_load_vector();

            // solve the linear system
            _solver.solve(_Keff, _feff, _u);

            // all done
            return;
        }

        // initialise effective stiffness matrix
        constexpr auto initialise_effective_stiffness_matrix() -> void
        {
            // duplicate the stiffness matrix pattern
            _Keff.duplicate(_K);

            // duplicate the stiffness matrix pattern
            _B.duplicate(_K);

            // all done
            return;
        }

        // compute effective stiffness matrix
        constexpr auto compute_effective_stiffness_matrix() -> void
        {
            // copy the stiffness matrix
            _Keff.copy_values(_K, linalg::sparsity_pattern::same);

            // scale the effective stiffness matrix by 1/2
            _Keff.scale(0.5);

            // add the capacitance matrix scaled by 1/dt to the effective stiffness matrix
            _Keff.add(1. / _dt, _M, linalg::sparsity_pattern::same);

            // copy the stiffness matrix into the effective RHS matrix
            _B.copy_values(_K, linalg::sparsity_pattern::same);

            // scale the effective rhs matrix by -1/2
            _B.scale(-0.5);

            // add the capacitance matrix scaled by 1/dt to the effective RHS matrix
            _B.add(1. / _dt, _M, linalg::sparsity_pattern::same);

            // all done
            return;
        };

        // compute effective load vector
        constexpr auto compute_effective_load_vector() -> void
        {
            // compute B * u_n and store it in the effective load vector
            _B.mult_add_vector(_u, _f, _feff);

            // all done
            return;
        }


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
        // the effective stiffness matrix, stores Keff = K + 1/dt * M
        matrix_type _Keff;
        // the effective RHS matrix, stores B = -1/2 * K + 1/dt * M
        matrix_type _B;
        // the effective load vector, stores feff = f + B * u_n
        vector_type _feff;
        // the solution vector
        vector_type _u;
    };


}    // namespace mito::timesteppers


// end of file
